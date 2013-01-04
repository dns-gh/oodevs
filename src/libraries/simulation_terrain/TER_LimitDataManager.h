// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef TER_LIMITDATAMANAGER_H
#define TER_LIMITDATAMANAGER_H

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

class TER_LimitData;

// A deduplicating allocator of TER_LimitData.
class TER_LimitDataManager : private boost::noncopyable
{
public:
    typedef boost::shared_ptr< TER_LimitData > T_LimitDataPtr;

public:
             TER_LimitDataManager();
    virtual ~TER_LimitDataManager();

    // Return a T_LimitData built from the input points. If the manager already
    // knows an existing instance built from the very same points sequence, it
    // returns it.
    T_LimitDataPtr CreateLimit( const T_PointVector& points );

    // Unregister and delete the limit instance.
    // DO NOT CALL this method manually, it will be executed by the shared_ptr
    // machinery automatically when created T_LimitDataPtr have to be released.
    void DeleteLimit( const TER_LimitData* limit );

    // Return true if the manager knows a T_LimitData created from points. For
    // tests purpose.
    bool HasLimit( const T_PointVector& points ) const;

private:
    typedef boost::weak_ptr< TER_LimitData > T_LimitDataWeakPtr;

private:
    std::map< T_PointVector, T_LimitDataWeakPtr > limits_;
};

#endif // TER_LIMITDATAMANAGER_H
