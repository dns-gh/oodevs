// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPositions_h_
#define __UrbanPositions_h_

#include "LocationPositions.h"

namespace urban
{
    class Drawer_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  UrbanPositions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public LocationPositions
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( const urban::TerrainObject_ABC& object, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanPositions( const UrbanPositions& );            //!< Copy constructor
    UrbanPositions& operator=( const UrbanPositions& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    static std::auto_ptr< urban::Drawer_ABC > drawer_;
    const urban::TerrainObject_ABC& object_;
    //@}
};

#endif // __UrbanPositions_h_
