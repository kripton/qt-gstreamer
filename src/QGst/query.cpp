/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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
#include "query.h"
#include "element.h"
#include "../QGlib/error.h"
#include "../QGlib/string_p.h"
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

QueryPtr Query::create(QueryType type, const StructureBase & structure)
{
    GstStructure *s = structure.isValid() ? gst_structure_copy(structure) : NULL;
    return QueryPtr::wrap(gst_query_new_application(static_cast<GstQueryType>(type), s), false);
}

QString Query::name() const
{
    return GST_QUERY_TYPE_NAME(object<GstQuery>());
}

QueryType Query::type() const
{
    return static_cast<QueryType>(GST_QUERY_TYPE(object<GstQuery>()));
}

SharedStructure Query::structure()
{
    return SharedStructure(gst_query_get_structure(object<GstQuery>()));
}

const SharedStructure Query::structure() const
{
    return SharedStructure(gst_query_get_structure(object<GstQuery>()));
}

void Query::ref()
{
    //We are not supposed to ref() query objects created with gst_query_new_* methods
    //Workaround while the global hash is not implemented for refcounting
    //gst_query_ref(object<GstQuery>());
}

void Query::unref()
{
    //We have to unref() the object only once
    //Workaround while the global hash is not implemented for refcounting, leak for now
    //gst_query_unref(object<GstQuery>());
}

//********************************************************

PositionQueryPtr PositionQuery::create(Format format)
{
    return PositionQueryPtr::wrap(gst_query_new_position(static_cast<GstFormat>(format)), false);
}

Format PositionQuery::format() const
{
    GstFormat f;
    gst_query_parse_position(object<GstQuery>(), &f, NULL);
    return static_cast<Format>(f);
}

qint64 PositionQuery::position() const
{
    gint64 p;
    gst_query_parse_position(object<GstQuery>(), NULL, &p);
    return p;
}

void PositionQuery::setPosition(Format format, qint64 position)
{
    gst_query_set_position(object<GstQuery>(), static_cast<GstFormat>(format), position);
}

//********************************************************

DurationQueryPtr DurationQuery::create(Format format)
{
    return DurationQueryPtr::wrap(gst_query_new_duration(static_cast<GstFormat>(format)), false);
}

Format DurationQuery::format() const
{
    GstFormat f;
    gst_query_parse_duration(object<GstQuery>(), &f, NULL);
    return static_cast<Format>(f);
}

qint64 DurationQuery::duration() const
{
    gint64 d;
    gst_query_parse_duration(object<GstQuery>(), NULL, &d);
    return d;
}

void DurationQuery::setDuration(Format format, qint64 duration)
{
    gst_query_set_duration(object<GstQuery>(), static_cast<GstFormat>(format), duration);
}

//********************************************************

LatencyQueryPtr LatencyQuery::create()
{
    return LatencyQueryPtr::wrap(gst_query_new_latency(), false);
}

bool LatencyQuery::hasLive() const
{
    gboolean l;
    gst_query_parse_latency(object<GstQuery>(), &l, NULL, NULL);
    return l;
}

ClockTime LatencyQuery::minimumLatency() const
{
    ClockTime c;
    gst_query_parse_latency(object<GstQuery>(), NULL, &c, NULL);
    return c;
}

ClockTime LatencyQuery::maximumLatency() const
{
    ClockTime c;
    gst_query_parse_latency(object<GstQuery>(), NULL, NULL, &c);
    return c;
}

void LatencyQuery::setLatency(bool live, ClockTime minimumLatency, ClockTime maximumLatency)
{
    gst_query_set_latency(object<GstQuery>(), live, minimumLatency, maximumLatency);
}

//********************************************************

SeekingQueryPtr SeekingQuery::create(Format format)
{
    return SeekingQueryPtr::wrap(gst_query_new_seeking(static_cast<GstFormat>(format)), false);
}

Format SeekingQuery::format() const
{
    GstFormat f;
    gst_query_parse_seeking(object<GstQuery>(), &f, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

bool SeekingQuery::seekable() const
{
    gboolean s;
    gst_query_parse_seeking(object<GstQuery>(), NULL, &s, NULL, NULL);
    return s;
}

qint64 SeekingQuery::segmentStart() const
{
    gint64 s;
    gst_query_parse_seeking(object<GstQuery>(), NULL, NULL, &s, NULL);
    return s;
}

qint64 SeekingQuery::segmentEnd() const
{
    gint64 s;
    gst_query_parse_seeking(object<GstQuery>(), NULL, NULL, NULL, &s);
    return s;
}

void SeekingQuery::setSeeking(Format format, bool seekable, qint64 segmentStart, qint64 segmentEnd)
{
    gst_query_set_seeking(object<GstQuery>(), static_cast<GstFormat>(format), seekable,
                          segmentStart, segmentEnd);
}

//********************************************************

SegmentQueryPtr SegmentQuery::create(Format format)
{
    return SegmentQueryPtr::wrap(gst_query_new_segment(static_cast<GstFormat>(format)), false);
}

double SegmentQuery::rate() const
{
    gdouble r;
    gst_query_parse_segment(object<GstQuery>(), &r, NULL, NULL, NULL);
    return r;
}

Format SegmentQuery::format() const
{
    GstFormat f;
    gst_query_parse_segment(object<GstQuery>(), NULL, &f, NULL, NULL);
    return static_cast<Format>(f);
}

qint64 SegmentQuery::startValue() const
{
    gint64 s;
    gst_query_parse_segment(object<GstQuery>(), NULL, NULL, &s, NULL);
    return s;
}

qint64 SegmentQuery::stopValue() const
{
    gint64 s;
    gst_query_parse_segment(object<GstQuery>(), NULL, NULL, NULL, &s);
    return s;
}

void SegmentQuery::setSegment(Format format, double rate, qint64 startValue, qint64 stopValue)
{
    gst_query_set_segment(object<GstQuery>(), rate, static_cast<GstFormat>(format), startValue,
                          stopValue);
}

//********************************************************

ConvertQueryPtr ConvertQuery::create(Format sourceFormat, qint64 value, Format destinationFormat)
{
    return ConvertQueryPtr::wrap(gst_query_new_convert(static_cast<GstFormat>(sourceFormat), value,
                                   static_cast<GstFormat>(destinationFormat)), false);
}

Format ConvertQuery::sourceFormat() const
{
    GstFormat f;
    gst_query_parse_convert(object<GstQuery>(), &f, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

qint64 ConvertQuery::sourceValue() const
{
    gint64 v;
    gst_query_parse_convert(object<GstQuery>(), NULL, &v, NULL, NULL);
    return v;
}

Format ConvertQuery::destinationFormat() const
{
    GstFormat f;
    gst_query_parse_convert(object<GstQuery>(), NULL, NULL, &f, NULL);
    return static_cast<Format>(f);
}

qint64 ConvertQuery::destinationValue() const
{
    gint64 v;
    gst_query_parse_convert(object<GstQuery>(), NULL, NULL, NULL, &v);
    return v;
}

void ConvertQuery::setConvert(Format sourceFormat, qint64 sourceValue, Format destinationFormat,
                              qint64 destinationValue)
{
    gst_query_set_convert(object<GstQuery>(), static_cast<GstFormat>(sourceFormat), sourceValue,
                          static_cast<GstFormat>(destinationFormat), destinationValue);
}

//********************************************************

FormatsQueryPtr FormatsQuery::create()
{
    return FormatsQueryPtr::wrap(gst_query_new_formats(), false);
}

QList<Format> FormatsQuery::formats()
{
    guint cnt;
    QList<Format> formats;
    gst_query_parse_formats_length(object<GstQuery>(), &cnt);
    GstFormat f;
    for (uint i=0; i<cnt; i++) {
        gst_query_parse_formats_nth(object<GstQuery>(), i, &f);
        formats << static_cast<Format>(f);
    }
    return formats;
}

void FormatsQuery::setFormats(QList<Format> formats)
{
    int cnt = formats.count();
    if (cnt==0) return;
    GstFormat f[cnt];
    for (int i=0; i<cnt; i++) {
        f[i] = static_cast<GstFormat>(formats.at(i));
    }
    gst_query_set_formatsv(object<GstQuery>(), cnt, f);
}

//********************************************************

BufferingQueryPtr BufferingQuery::create(Format format)
{
    return BufferingQueryPtr::wrap(gst_query_new_buffering(static_cast<GstFormat>(format)), false);
}

bool BufferingQuery::isBusy() const
{
    gboolean b;
    gst_query_parse_buffering_percent(object<GstQuery>(), &b, NULL);
    return b;
}

int BufferingQuery::percent() const
{
    gint p;
    gst_query_parse_buffering_percent(object<GstQuery>(), NULL, &p);
    return p;
}

void BufferingQuery::setPercent(bool busy, int percent)
{
    gst_query_set_buffering_percent(object<GstQuery>(), busy, percent);
}

BufferingMode BufferingQuery::mode() const
{
    GstBufferingMode m;
    gst_query_parse_buffering_stats(object<GstQuery>(), &m, NULL, NULL, NULL);
    return static_cast<BufferingMode>(m);
}

int BufferingQuery::averageIn() const
{
    gint a;
    gst_query_parse_buffering_stats(object<GstQuery>(), NULL, &a, NULL, NULL);
    return a;
}

int BufferingQuery::averageOut() const
{
    gint a;
    gst_query_parse_buffering_stats(object<GstQuery>(), NULL, NULL, &a, NULL);
    return a;

}

qint64 BufferingQuery::bufferingLeft() const
{
    gint64 l;
    gst_query_parse_buffering_stats(object<GstQuery>(), NULL, NULL, NULL, &l);
    return l;
}
;
void BufferingQuery::setStats(BufferingMode mode, int averageIn, int averageOut, qint64 bufferingLeft)
{
    gst_query_set_buffering_stats(object<GstQuery>(), static_cast<GstBufferingMode>(mode), averageIn, averageOut, bufferingLeft);
}

Format BufferingQuery::format() const
{
    GstFormat f;
    gst_query_parse_buffering_range(object<GstQuery>(), &f, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

qint64 BufferingQuery::rangeStart() const
{
    gint64 r;
    gst_query_parse_buffering_range(object<GstQuery>(), NULL, &r, NULL, NULL);
    return r;
}

qint64 BufferingQuery::rangeStop() const
{
    gint64 r;
    gst_query_parse_buffering_range(object<GstQuery>(), NULL, NULL, &r, NULL);
    return r;
}

qint64 BufferingQuery::estimatedTotal() const
{
    gint64 r;
    gst_query_parse_buffering_range(object<GstQuery>(), NULL, NULL, NULL, &r);
    return r;
}

void BufferingQuery::setRange(Format rangeFormat, qint64 rangeStart, qint64 rangeStop,
                              qint64 estimatedTotal)
{
    gst_query_set_buffering_range(object<GstQuery>(), static_cast<GstFormat>(rangeFormat),
                                  rangeStart, rangeStop, estimatedTotal);
}

//********************************************************

UriQueryPtr UriQuery::create()
{
    return UriQueryPtr::wrap(gst_query_new_uri(), false);
}

QUrl UriQuery::uri() const
{
    gchar *uri;
    gst_query_parse_uri(object<GstQuery>(), &uri);
    return QUrl::fromPercentEncoding(uri);
}

void UriQuery::setUri(const QUrl & uri)
{
    gst_query_set_uri(object<GstQuery>(), uri.toEncoded());
}

//********************************************************

} //namespace QGst

QGLIB_REGISTER_VALUEIMPL_IMPLEMENTATION(
    QGst::QueryPtr,
    QGst::QueryPtr::wrap(GST_QUERY(gst_value_get_mini_object(value))),
    gst_value_set_mini_object(value, GST_MINI_OBJECT(static_cast<GstQuery*>(data)))
)

QDebug operator<<(QDebug debug, QGst::QueryType type)
{
    debug.nospace() << gst_query_type_get_name(static_cast<GstQueryType>(type));
    return debug.space();
}

QDebug operator<<(QDebug debug, const QGst::QueryPtr & query)
{
    debug.nospace() << "QGst::Query(Type: " << query->type() << ")";
    return debug.space();
}

