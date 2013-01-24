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
    class GlTools_ABC;
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
    explicit SimpleLocationDrawer( const GlTools_ABC& tools );
    virtual ~SimpleLocationDrawer();
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines     ( const T_PointVector& points );
    virtual void VisitRectangle ( const T_PointVector& points );
    virtual void VisitPolygon   ( const T_PointVector& points );
    virtual void VisitCircle    ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint     ( const geometry::Point2f& point );
    virtual void VisitPath      ( const geometry::Point2f& first, const T_PointVector& points );
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
    const GlTools_ABC& tools_;
    //@}
};

}

#endif // CLIENTS_GUI_SIMPLELOCATIONDRAWER_H__
