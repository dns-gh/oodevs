// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_GLToolTipLine_h_
#define __MT_GLToolTipLine_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

// =============================================================================
/** @class  MT_GLToolTipLine
    @brief  MT_GLToolTipLine
    @par    Using example
    @code
    MT_GLToolTipLine;
    @endcode
*/
// Created: HME 2005-11-28
// =============================================================================
class MT_GLToolTipLine
{

public:
    //! @name Constructors/Destructor
    //@{
    MT_GLToolTipLine( QString strText, QFont font, GFX_Color color );
    virtual ~MT_GLToolTipLine();

    void Draw( QGLWidget* pWidget, MT_Rect viewRect, MT_Vector2D pos ) const ;

    uint  GetWidth() const;
    uint  GetHeight() const;
    uint  GetSpace() const;

private:
    QString         strText_;
    QFont           font_;
    GFX_Color       color_;
    QFontMetrics    fm_;
};



#endif // __MT_GLToolTipLine_h_
