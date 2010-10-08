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

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC; // useful?
    class LocationVisitor_ABC;
}

namespace urban
{
    class Drawer_ABC;
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  UrbanPositions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::Positions
                     , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( const urban::TerrainObject_ABC& object/*, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location*/ );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f, float adaptiveFactor = 1.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual bool CanAggregate() const;
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
    const urban::TerrainObject_ABC& object_;
    static std::auto_ptr< urban::Drawer_ABC > drawer_;
    //@}
};

#endif // __UrbanPositions_h_
