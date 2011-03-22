// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DisaggregationArea_ABC_h_
#define __DisaggregationArea_ABC_h_

#include "geometry/Types.h"
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  DisaggregationArea_ABC
    @brief  DisaggregationArea_ABC
*/
// Created: SBO 2011-03-16
// =============================================================================
class DisaggregationArea_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DisaggregationArea_ABC() {}
    virtual ~DisaggregationArea_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const geometry::Point2d& position ) const = 0;
    //@}
};
}
}

#endif // __DisaggregationArea_ABC_h_
