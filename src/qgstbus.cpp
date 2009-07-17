/*
    Copyright (C) 2009  George Kiagiadakis <kiagiadakis.george@gmail.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "qgstbus.h"
//the gstreamer headers are broken. we can't include <gst/gstbus.h> here
#include <gst/gst.h>

namespace QtGstreamer {

class QGstBusPrivate
{
public:
    static void bus_message(GstBus *bus, GstMessage *message, QGstBus *self);
};

//static
void QGstBusPrivate::bus_message(GstBus *bus, GstMessage *message, QGstBus *self)
{
    emit self->message(message);
}

QGstBus::QGstBus(GstBus *gstBus)
    : QGstObject(GST_OBJECT(gstBus))
{
    g_signal_connect(gstBus, "message", G_CALLBACK(&QGstBusPrivate::bus_message), this);
}

//static
QGstBusPtr QGstBus::fromGstBus(GstBus *gstBus)
{
    return QGstBusPtr(new QGstBus(gstBus));
}

QGstBus::~QGstBus()
{
}

void QGstBus::addSignalWatch()
{
    gst_bus_add_signal_watch(GST_BUS(m_object));
}

void QGstBus::removeSignalWatch()
{
    gst_bus_remove_signal_watch(GST_BUS(m_object));
}

}

#include "qgstbus.moc"
