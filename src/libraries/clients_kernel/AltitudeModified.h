// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModified_h_
#define __AltitudeModified_h_

#include <boost/noncopyable.hpp>
#include <geometry/types.h>

// =============================================================================
/** @class  AltitudeModified
    @brief  AltitudeModified
*/
// Created: JSR 2011-05-18
// =============================================================================
class AltitudeModified : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    AltitudeModified( const geometry::Polygon2f& polygon )
        : polygon_( polygon)
    {
        // NOTHING
    }
    virtual ~AltitudeModified()
    {
        // NOTHING
    }
    //@}

public:
    //! @name Member data
    //@{
    const geometry::Polygon2f& polygon_;
    //@}
};

#endif // __AltitudeModified_h_
