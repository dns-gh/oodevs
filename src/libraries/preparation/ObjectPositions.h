// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class Location_ABC;
    class ObjectType;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public kernel::Moveable_ABC
                      , public kernel::LocationVisitor_ABC
                      , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type, const kernel::Location_ABC& location );
             ObjectPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type );
    virtual ~ObjectPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Move( const geometry::Point2f& position );
    void Translate( const geometry::Point2f& position, const geometry::Vector2f& translation, float precision);

    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual bool CanAggregate() const;

    void Serialize( xml::xostream& xos ) const;
    std::string GetSymbol() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis, const kernel::ObjectType& type );
    void ReadPoint( xml::xistream& xis );
    void Update();
    virtual void VisitLines    ( const T_PointVector& points );
    virtual void VisitRectangle( const T_PointVector& points );
    virtual void VisitPolygon  ( const T_PointVector& points );
    virtual void VisitCircle   ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint    ( const geometry::Point2f& point );
    virtual void VisitPath     ( const geometry::Point2f& first, const T_PointVector& points );
    virtual void VisitCurve    ( const T_PointVector& points );
    virtual void VisitText     ( const QString& text, const QFont& font, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectType& type_;
    kernel::Location_ABC* location_;
    std::string symbol_;
    geometry::Rectangle2f boundingBox_;
    T_PointVector points_;
    //@}

};

#endif // __ObjectPositions_h_
