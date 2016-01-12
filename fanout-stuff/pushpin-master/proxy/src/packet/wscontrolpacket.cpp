/*
 * Copyright (C) 2014 Fanout, Inc.
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

#include "wscontrolpacket.h"

#include <assert.h>

QVariant WsControlPacket::toVariant() const
{
	QVariantHash obj;

	QVariantList vitems;
	foreach(const Item &item, items)
	{
		QVariantHash vitem;

		vitem["cid"] = item.cid;

		QByteArray typeStr;
		switch(item.type)
		{
			case Item::Here:   typeStr = "here"; break;
			case Item::Gone:   typeStr = "gone"; break;
			case Item::Grip:   typeStr = "grip"; break;
			case Item::Cancel: typeStr = "cancel"; break;
			case Item::Send:   typeStr = "send"; break;
			case Item::Detach: typeStr = "detach"; break;
			default:
				assert(0);
		}
		vitem["type"] = typeStr;

		if(!item.contentType.isEmpty())
			vitem["content-type"] = item.contentType;

		if(!item.message.isNull())
			vitem["message"] = item.message;

		if(!item.channelPrefix.isEmpty())
			vitem["channel-prefix"] = item.channelPrefix;

		vitems += vitem;
	}

	obj["items"] = vitems;
	return obj;
}

bool WsControlPacket::fromVariant(const QVariant &in)
{
	if(in.type() != QVariant::Hash)
		return false;

	QVariantHash obj = in.toHash();

	if(!obj.contains("items") || obj["items"].type() != QVariant::List)
		return false;
	QVariantList vitems = obj["items"].toList();

	items.clear();
	foreach(const QVariant &v, vitems)
	{
		if(v.type() != QVariant::Hash)
			return false;

		QVariantHash vitem = v.toHash();

		Item item;

		if(!vitem.contains("cid") || vitem["cid"].type() != QVariant::ByteArray)
			return false;
		item.cid = vitem["cid"].toByteArray();

		if(!vitem.contains("type") || vitem["type"].type() != QVariant::ByteArray)
			return false;
		QByteArray typeStr = vitem["type"].toByteArray();

		if(typeStr == "here")
			item.type = Item::Here;
		else if(typeStr == "gone")
			item.type = Item::Gone;
		else if(typeStr == "grip")
			item.type = Item::Grip;
		else if(typeStr == "cancel")
			item.type = Item::Cancel;
		else if(typeStr == "send")
			item.type = Item::Send;
		else if(typeStr == "detach")
			item.type = Item::Detach;
		else
			return false;

		if(vitem.contains("content-type"))
		{
			if(vitem["content-type"].type() != QVariant::ByteArray)
				return false;

			QByteArray contentType = vitem["content-type"].toByteArray();
			if(!contentType.isEmpty())
				item.contentType = contentType;
		}

		if(vitem.contains("message"))
		{
			if(vitem["message"].type() != QVariant::ByteArray)
				return false;

			item.message = vitem["message"].toByteArray();
		}

		if(vitem.contains("channel-prefix"))
		{
			if(vitem["channel-prefix"].type() != QVariant::ByteArray)
				return false;

			QByteArray channelPrefix = vitem["channel-prefix"].toByteArray();
			if(!channelPrefix.isEmpty())
				item.channelPrefix = channelPrefix;
		}

		items += item;
	}

	return true;
}
