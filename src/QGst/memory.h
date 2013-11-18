/*
    Copyright (C) 2013  Diane Trout <diane@ghic.org

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QGST_MEMORY_H
#define QGST_MEMORY_H

#include "global.h"
#include "miniobject.h"

namespace QGst {

    /*! \headerfile memory.h <QGst/Memory>
     *  \brief Wrapper class for GstMemory
     *
     * GstMemory is a lightweight refcounted object that wraps a region 
     * of memory. They are typically used to manage the data of a 
     * GstBuffer.
     */

// class QTGSTREAMER_EXPORT MapInfo
// {
// private:
//     struct Data;
//     QSharedDataPointer<Data> d;
// };

class QTGSTREAMER_EXPORT Memory : public MiniObject
{
    QGST_WRAPPER(Memory)
public:
    static MemoryPtr create(MemoryFlags flags, void *allocator, MemoryPtr parent, size_t maxsize, size_t align, size_t offset, size_t size);

    size_t getSizes(size_t &offset, size_t &maxsize);

    // bool map(MapInfo &info, MapFlags flags);
    // void unmap(MapInfo &info);
};

} // namespace QGst

QGST_REGISTER_TYPE(QGst::Memory)

#endif
