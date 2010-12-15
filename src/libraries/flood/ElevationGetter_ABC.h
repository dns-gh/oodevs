// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ElevationGetter_ABC_h_
#define __ElevationGetter_ABC_h_

#include <geometry/types.h>
#include <boost/noncopyable.hpp>

namespace flood
{
// =============================================================================
/** @class  ElevationGetter_ABC
    @brief  ElevationGetter_ABC
*/
// Created: JSR 2010-12-15
// =============================================================================
class ElevationGetter_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ElevationGetter_ABC() {}
    virtual ~ElevationGetter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual short GetElevationAt( const geometry::Point2f& point ) const = 0;
    //@}
};

}

#endif // __ElevationGetter_ABC_h_
