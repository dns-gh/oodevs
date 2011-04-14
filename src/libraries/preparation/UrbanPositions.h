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

namespace urban
{
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
    explicit UrbanPositions( const urban::TerrainObject_ABC& object );
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
    virtual bool IsAggregated() const;
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
    //@}
};

#endif // __UrbanPositions_h_
