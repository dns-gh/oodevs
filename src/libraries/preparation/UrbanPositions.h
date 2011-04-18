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

#include "clients_kernel/UrbanPositions_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban oositions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanPositions( const urban::TerrainObject_ABC& object );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f Barycenter() const;
    virtual geometry::Rectangle2f BoundingBox() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC& object_;
    //@}
};

#endif // __UrbanPositions_h_
