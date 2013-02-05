// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SpatialIndexer_h_
#define __SpatialIndexer_h_

#include <geometry/Types.h>

namespace kernel
{
    class UrbanObject_ABC;
}

namespace geostore
{

class SpatialIndexer
{
public:
    virtual ~SpatialIndexer() {}

    virtual void GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< const kernel::UrbanObject_ABC* >& result ) const = 0;
};

}

#endif /* __SpatialIndexer_h_ */
