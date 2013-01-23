// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef TER_LIMIT_DATA_MANAGER_H
#define TER_LIMIT_DATA_MANAGER_H

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class TER_LimitData;

// A deduplicating allocator of TER_LimitData.
class TER_LimitDataManager : private boost::noncopyable
{
public:
    // Return a TER_LimitData built from the input points.
    // If the manager already knows of an existing instance built from the very
    // same points sequence, it returns it.
    boost::shared_ptr< TER_LimitData > CreateLimit( const T_PointVector& points );

    // Return true if the manager knows a TER_LimitData created from points.
    // For tests purpose.
    bool HasLimit( const T_PointVector& points ) const;

private:
    // Unregister and delete the limit instance.
    void DeleteLimit( const TER_LimitData* limit );

private:
    std::map< T_PointVector, boost::weak_ptr< TER_LimitData > > limits_;
};

#endif // TER_LIMIT_DATA_MANAGER_H
