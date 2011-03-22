// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DisaggregationArea_h_
#define __DisaggregationArea_h_

#include "DisaggregationArea_ABC.h"

namespace sword
{
    class ShapeCreation;
}

namespace plugins
{
namespace vrforces
{
    class Facade;

// =============================================================================
/** @class  DisaggregationArea
    @brief  DisaggregationArea
*/
// Created: SBO 2011-03-16
// =============================================================================
class DisaggregationArea : public DisaggregationArea_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DisaggregationArea( const sword::ShapeCreation& message, Facade& facade );
    virtual ~DisaggregationArea();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const geometry::Point2d& position ) const;
    //@}

private:
    //! @name Member data
    //@{
    const geometry::Polygon2d polygon_;
    //@}
};
}
}

#endif // __DisaggregationArea_h_
