// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerShapePoint_h_
#define __DrawerShapePoint_h_

#include "DrawerShape.h"

namespace gui
{

// =============================================================================
/** @class  DrawerShapePoint
    @brief  DrawerShapePoint
*/
// Created: SBO 2007-03-07
// =============================================================================
class DrawerShapePoint : public DrawerShape
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShapePoint( const DrawerStyle& style, const QColor& color );
    virtual ~DrawerShapePoint();
    //@}

    //! @name Operations
    //@{
    virtual void AddPoint( const geometry::Point2f& point );
    virtual void Draw( const geometry::Rectangle2f& viewport, const QColor& color, bool overlined ) const;
    //@}
};

}

#endif // __DrawerShapePoint_h_
