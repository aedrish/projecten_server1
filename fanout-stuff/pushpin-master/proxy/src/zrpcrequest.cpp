/*
 * Copyright (C) 2014-2015 Fanout, Inc.
 *
 * This file is part of Pushpin.
 *
 * Pushpin is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Pushpin is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "zrpcrequest.h"

#include <assert.h>
#include <QTimer>
#include "packet/zrpcrequestpacket.h"
#include "packet/zrpcresponsepacket.h"
#include "zrpcmanager.h"
#include "uuidutil.h"
#include "log.h"

class ZrpcRequest::Private : public QObject
{
	Q_OBJECT

public:
	ZrpcRequest *q;
	ZrpcManager *manager;
	QByteArray id;
	QString method;
	QVariantHash args;
	bool success;
	QVariant result;
	ErrorCondition condition;
	QTimer *timer;

	Private(ZrpcRequest *_q) :
		QObject(_q),
		q(_q),
		manager(0),
		success(false),
		condition(ErrorGeneric),
		timer(0)
	{
	}

	~Private()
	{
		cleanup();
	}

	void cleanup()
	{
		if(timer)
		{
			timer->disconnect(this);
			timer->setParent(0);
			timer->deleteLater();
			timer = 0;
		}

		if(manager)
		{
			manager->unlink(q);
			manager = 0;
		}
	}

	void respond(const QVariant &value)
	{
		ZrpcResponsePacket p;
		p.id = id;
		p.success = true;
		p.value = value;
		manager->write(p);
	}

	void respondError(const QByteArray &condition)
	{
		ZrpcResponsePacket p;
		p.id = id;
		p.success = false;
		p.condition = condition;
		manager->write(p);
	}

	void handle(const ZrpcRequestPacket &packet)
	{
		id = packet.id;
		method = packet.method;
		args = packet.args;
	}

	void handle(const ZrpcResponsePacket &packet)
	{
		cleanup();

		success = packet.success;
		if(success)
		{
			result = packet.value;
			q->onSuccess();
		}
		else
		{
			// TODO: packet.condition
			q->onError();
		}

		emit q->finished();
	}

private slots:
	void doStart()
	{
		if(!manager->canWriteImmediately())
		{
			success = false;
			condition = ErrorUnavailable;
			cleanup();
			emit q->finished();
			return;
		}

		ZrpcRequestPacket p;
		p.id = id;
		p.method = method;
		p.args = args;

		if(manager->timeout() >= 0)
		{
			timer = new QTimer(this);
			connect(timer, SIGNAL(timeout()), SLOT(timer_timeout()));
			timer->setSingleShot(true);
			timer->start(manager->timeout());
		}

		manager->write(p);
	}

	void timer_timeout()
	{
		success = false;
		condition = ErrorTimeout;
		cleanup();
		emit q->finished();
	}
};

ZrpcRequest::ZrpcRequest(QObject *parent) :
	QObject(parent)
{
	d = new Private(this);
}

ZrpcRequest::ZrpcRequest(ZrpcManager *manager, QObject *parent) :
	QObject(parent)
{
	d = new Private(this);
	setupClient(manager);
}

ZrpcRequest::~ZrpcRequest()
{
	delete d;
}

QByteArray ZrpcRequest::id() const
{
	return d->id;
}

QString ZrpcRequest::method() const
{
	return d->method;
}

QVariantHash ZrpcRequest::args() const
{
	return d->args;
}

bool ZrpcRequest::success() const
{
	return d->success;
}

QVariant ZrpcRequest::result() const
{
	return d->result;
}

ZrpcRequest::ErrorCondition ZrpcRequest::errorCondition() const
{
	return d->condition;
}

void ZrpcRequest::start(const QString &method, const QVariantHash &args)
{
	d->method = method;
	d->args = args;
	QMetaObject::invokeMethod(d, "doStart", Qt::QueuedConnection);
}

void ZrpcRequest::respond(const QVariant &value)
{
	d->respond(value);
}

void ZrpcRequest::respondError(const QByteArray &condition)
{
	d->respondError(condition);
}

void ZrpcRequest::setError(ErrorCondition condition)
{
	d->success = false;
	d->condition = condition;
}

void ZrpcRequest::onSuccess()
{
	// by default, do nothing
}

void ZrpcRequest::onError()
{
	// by default, do nothing
}

void ZrpcRequest::setupClient(ZrpcManager *manager)
{
	d->id = UuidUtil::createUuid();
	d->manager = manager;
	d->manager->link(this);
}

void ZrpcRequest::setupServer(ZrpcManager *manager)
{
	d->manager = manager;
}

void ZrpcRequest::handle(const ZrpcRequestPacket &packet)
{
	assert(d->manager);

	d->handle(packet);
}

void ZrpcRequest::handle(const ZrpcResponsePacket &packet)
{
	assert(d->manager);

	d->handle(packet);
}

#include "zrpcrequest.moc"
