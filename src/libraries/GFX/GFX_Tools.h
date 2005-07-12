//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Tools.h $
// $Author: Age $
// $Modtime: 31/03/05 16:22 $
// $Revision: 11 $
// $Workfile: GFX_Tools.h $
//
//*****************************************************************************

#ifndef __GFX_Tools_h_
#define __GFX_Tools_h_

#include "GFX.h"
#include "MT_Tools/MT_Vector3D.h"

class GFX_Color;
class GFX_Text;

//*****************************************************************************
// Created: FBD 02-11-22
//*****************************************************************************
class GFX_Tools
{
public:
    explicit GFX_Tools();
    virtual ~GFX_Tools();
    //-------------------------------------------------------------------------
    /** @name Text Tools*/
    //-------------------------------------------------------------------------
    //@{
    static void InitializeFonts       ();
    static void Print                 ( MT_Vector2D vPos, MT_Float rSize, const GFX_Color& color, bool bCenter, const char *fmt, ...);
    static void PrintWithShadows      ( MT_Vector2D vPos, MT_Float rSize, MT_Float rSizeShadow, MT_Float rStrengthShadow, const GFX_Color& color, bool bCenter, const char *fmt, ...);
    static void DeleteFonts           ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    static void CreateGLAgentShadow   ( MT_Vector2D vPos, MT_Float rSize, MT_Float rSizeShadow, MT_Float rStrengthShadow, const GFX_Color& color, bool bCenter, const std::string& sUnit, MT_Float rPercentLife );
    static bool IsInsideGLAgent       ( const MT_Vector2D& vPosAgent, MT_Float rSize, const std::string& sUnit, const MT_Vector2D& vPos );
    static void CreateGLLines         ( CT_LinePtrVector&     lineVector    , MT_Float rWidth, const GFX_Color& color );
    static void CreateGLLines         ( CT_PointVector&       pointVector   , MT_Float rWidth, const GFX_Color& color );
    static void CreateGLLine          ( const MT_Line&        line          , MT_Float rWidth, const GFX_Color& color );
    static void CreateGLLine          ( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, MT_Float rWidth, const GFX_Color& color );
    static void CreateGLLine3D        ( const MT_Vector3D& vPos1, const MT_Vector3D& vPos2, MT_Float rWidth, const GFX_Color& color );
    static void CreateGLTriangles     ( CT_TrianglePtrVector& triangleVector, MT_Float rWidth, const GFX_Color& color );
    static void CreateGLTrianglesLines( CT_TrianglePtrVector& triangleVector, MT_Float rWidth, const GFX_Color& color );
    static void CreateGLRectPoly      ( const MT_Rect& rect, const GFX_Color& color );
    static void CreateGLRect          ( const MT_Rect& rect, MT_Float rWidth, const GFX_Color& color );
    static void CreateGLButton        ( const MT_Rect& rect, const GFX_Color& color, MT_Float rSizeBorder );
    static void CreateGLCross         ( const MT_Vector2D& vCenter, MT_Float rRadius, MT_Float rWidth, const GFX_Color& color );
    static void ClipBordersRect       ( const MT_Rect& rect );
    static void CreateGLCircle        ( const MT_Vector2D& center, MT_Float radius, const GFX_Color& color );
    static void CreateGLArc           ( const MT_Vector2D& center, MT_Float radius, MT_Float beginAngle, MT_Float endAngle, const GFX_Color& color );
    static void CreateGLArrow         ( const MT_Vector2D& src, const MT_Vector2D& dest, const GFX_Color& color, MT_Float rSize = 1.0, MT_Float rSizeArrow = 10. );
    static void CreateGLTriangleShadow( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3, MT_Float rWidth, MT_Float rStrengthShadow, const GFX_Color& color );
    static void CreateGLTriangle      ( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3, const GFX_Color& color );
    static void CreateGLTriangle3D    ( const MT_Vector3D& vPos1, const MT_Vector3D& vPos2, const MT_Vector3D& vPos3, const GFX_Color& color );

    //@}
    
    //-------------------------------------------------------------------------
    /** @name Colors*/
    //-------------------------------------------------------------------------
    //@{
    static GFX_Color         GetColorForTeam        ( int nTeamId );
    static ulong             ColorScaleHSL          ( const ulong Col1, const ulong Col2, const MT_Float Ratio );
    static void              RGBtoHLS               ( const ulong rgb, MT_Float* H, MT_Float* L, MT_Float *S );
    static ulong             HLStoRGB               ( const MT_Float H, const MT_Float L, const MT_Float S );
    static MT_Float          HueToRGB               ( const MT_Float m1, const MT_Float m2, MT_Float h );

    //@}

private:
	static GFX_Text				    fontAPP6FR_;
	static GFX_Text				    fontText_;
    
};

#ifdef USE_INLINE
#	include "GFX_Tools.inl"
#endif


#endif // __GFX_Tools_h_