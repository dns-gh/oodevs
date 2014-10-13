// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_SIMPLELOCATIONDRAWER_H__
#define CLIENTS_GUI_SIMPLELOCATIONDRAWER_H__

#include "clients_kernel/LocationVisitor_ABC.h"

namespace gui
{
    class GLView_ABC;
}

namespace gui
{
// =============================================================================
/** @class  SimpleLocationDrawer
    @brief  SimpleLocationDrawer
*/
// Created: SBO 2008-05-30
// =============================================================================
class SimpleLocationDrawer : public kernel::LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleLocationDrawer( const GLView_ABC& tools );
             SimpleLocationDrawer( const GLView_ABC& tools, const QColor& color );
    virtual ~SimpleLocationDrawer();
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines     ( const T_PointVector& points );
    virtual void VisitRectangle ( const T_PointVector& points );
    virtual void VisitPolygon   ( const T_PointVector& points );
    virtual void VisitCircle    ( const geometry::Point2f& center, float radius );
    virtual void VisitCurve     ( const T_PointVector& points );
    virtual void VisitPoint     ( const geometry::Point2f& point );
    virtual void VisitPath      ( const geometry::Point2f& first, const T_PointVector& points );
    virtual void VisitText      ( const QString& text, const QFont& font, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleLocationDrawer( const SimpleLocationDrawer& );            //!< Copy constructor
    SimpleLocationDrawer& operator=( const SimpleLocationDrawer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const GLView_ABC& tools_;
    QColor color_;
    //@}
};

}

#endif // CLIENTS_GUI_SIMPLELOCATIONDRAWER_H__
