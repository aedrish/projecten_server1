/*
 * Copyright (C) 2015 Fanout, Inc.
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

#ifndef ACCEPTREQUEST_H
#define ACCEPTREQUEST_H

#include <QObject>
#include "packet/httpresponsedata.h"
#include "zrpcrequest.h"

class AcceptData;
class ZrpcManager;

class AcceptRequest : public ZrpcRequest
{
	Q_OBJECT

public:
	class ResponseData
	{
	public:
		bool accepted;
		HttpResponseData response;

		ResponseData() :
			accepted(false)
		{
		}
	};

	AcceptRequest(ZrpcManager *manager, QObject *parent = 0);
	~AcceptRequest();

	ResponseData result() const;

	void start(const AcceptData &adata);

protected:
	virtual void onSuccess();

private:
	class Private;
	Private *d;
};

#endif
