// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerShapePolygon_h_
#define __DrawerShapePolygon_h_

#include "DrawerShape.h"

namespace gui
{

// =============================================================================
/** @class  DrawerShapePolygon
    @brief  DrawerShapePolygon
*/
// Created: SBO 2007-03-07
// =============================================================================
class DrawerShapePolygon : public DrawerShape
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShapePolygon( const DrawerStyle& style, const QColor& color );
    virtual ~DrawerShapePolygon();
    //@}

    //! @name Operations
    //@{
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void PopPoint();
    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawerShapePolygon( const DrawerShapePolygon& );            //!< Copy constructor
    DrawerShapePolygon& operator=( const DrawerShapePolygon& ); //!< Assignment operator
    //@}
};

}

#endif // __DrawerShapePolygon_h_
