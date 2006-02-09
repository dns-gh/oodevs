// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_GLToolTip_h_
#define __MT_GLToolTip_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
class MT_GLToolTipLine;

// =============================================================================
/** @class  MT_GLToolTip
*/
// Created: HME 2005-11-25
// =============================================================================
class MT_GLToolTip
{

public:
             MT_GLToolTip( QGLWidget* pWidget );
    virtual ~MT_GLToolTip();

    void SetBackgroundColor( MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha );
    void SetBackgroundColor( GFX_Color color );
    
    void AddLine( std::string strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold = false, float rStretchFactor = 1 );
    void AddLine( std::string strText, GFX_Color color, bool bBold = false, float rStretchFactor = 1 );
    void AddLine( QString strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold = false, float rStretchFactor = 1 );
    void AddLine( QString strText, GFX_Color color, bool bBold = false, float rStretchFactor = 1 );

    void InsertLine( std::string strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold = false, float rStretchFactor = 1 );
    void InsertLine( std::string strText, GFX_Color color, bool bBold = false, float rStretchFactor = 1 );
    void InsertLine( QString strText, MT_Float rRed, MT_Float rGreen, MT_Float rBlue, MT_Float rAlpha, bool bBold = false, float rStretchFactor = 1 );
    void InsertLine( QString strText, GFX_Color color, bool bBold = false, float rStretchFactor = 1 );

    
    void Draw( MT_Rect viewRect, MT_Vector2D pos, MT_Float zoom = 1 ) const ;

private:
    typedef std::vector< MT_GLToolTipLine >     LinesVector;
    typedef LinesVector::iterator               ITLinesVector;
    typedef LinesVector::const_iterator         CITLinesVector;

private:
    LinesVector        lines_;
    MT_Float           rWidth_;
    MT_Float           rHeight_;
    QGLWidget*         pWidget_;
    GFX_Color          backgroundColor_;
};



#endif // __MT_GLToolTip_h_
