// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DisaggregationStrategy_ABC_h_
#define __DisaggregationStrategy_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ShapeCreation;
    class ShapeDestruction;
}

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  DisaggregationStrategy_ABC
    @brief  Disaggregation strategy definition
*/
// Created: SBO 2011-03-16
// =============================================================================
    class DisaggregationStrategy_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DisaggregationStrategy_ABC() {}
    virtual ~DisaggregationStrategy_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsAggregatedLocation( double latitude, double longitude ) const = 0;
    virtual void AddArea( const sword::ShapeCreation& message ) = 0;
    virtual void RemoveArea( const sword::ShapeDestruction& message ) = 0;
    //@}
};
}
}

#endif // __DisaggregationStrategy_ABC_h_
