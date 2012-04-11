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

namespace xml
{
    class xistream;
}

namespace kernel
{
    class UrbanColor_ABC;
    class CoordinateConverter_ABC;
    class Architecture_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( xml::xistream& xis, const std::string& name, const kernel::UrbanColor_ABC& pColor,
                             const kernel::CoordinateConverter_ABC& converter, const kernel::Architecture_ABC& architecture );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual const geometry::Point2f& Barycenter() const;
    virtual const geometry::Rectangle2f& BoundingBox() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual float ComputeArea() const;
    virtual void ToggleSelection();
    virtual void SetInfrastructurePresent();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis, std::vector< geometry::Point2f >& positions, const kernel::CoordinateConverter_ABC& converter ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const kernel::UrbanColor_ABC& color_;
    bool selected_;
    unsigned int height_;
    bool hasInfrastructure_;
    geometry::Polygon2f polygon_;
    geometry::Point2f barycenter_;
    geometry::Rectangle2f boundingBox_;
    float area_;
    //@}
};

#endif // __UrbanPositions_h_
