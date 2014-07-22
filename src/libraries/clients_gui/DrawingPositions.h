// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_DrawingPositions_h_
#define __gui_DrawingPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/LocationProxy.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace gui
{

// =============================================================================
/** @class  DrawingPositions
    @brief  DrawingPositions
*/
// Created: SBO 2008-06-02
// =============================================================================
class DrawingPositions : public kernel::Positions
                       , public kernel::LocationProxy
                       , public kernel::LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingPositions();
    virtual ~DrawingPositions();
    //@}

    //! @name Operations
    //@{
    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );

    virtual void Update();
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingPositions( const DrawingPositions& );            //!< Copy constructor
    DrawingPositions& operator=( const DrawingPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

    //! @name Update
    //@{
    virtual void VisitLines    ( const T_PointVector& points );
    virtual void VisitRectangle( const T_PointVector& points );
    virtual void VisitPolygon  ( const T_PointVector& points );
    virtual void VisitCircle   ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint    ( const geometry::Point2f& point );
    virtual void VisitPath     ( const geometry::Point2f& point, const T_PointVector& points );
    virtual void VisitCurve    ( const T_PointVector& points );
    virtual void VisitText     ( const QString& text, const QFont& font, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f position_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

}

#endif // __gui_DrawingPositions_h_
