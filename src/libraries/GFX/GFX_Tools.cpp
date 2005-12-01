//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/GFX/src/GFX_Tools.cpp $
// $Author: Age $
// $Modtime: 10/05/05 14:10 $
// $Revision: 30 $
// $Workfile: GFX_Tools.cpp $
//
//*****************************************************************************

#include "GFX_pch.h"

#include "GFX_Tools.h"

#ifndef USE_INLINE
#	include "GFX_Tools.inl"
#endif

#include <qgl.h>

#include "GFX_Color.h"
#include "GFX_Text.h"


GFX_Text	GFX_Tools::fontAPP6FR_  ( "Scipio", 0.005, DEFAULT_CHARSET, true, FW_THIN  );
GFX_Text	GFX_Tools::fontText_    ( "Comic Sans MS", 0.0, ANSI_CHARSET, true, FW_BOLD );

#define DefMakeRGB(r,g,b)    (                 ((r)&0xFF)<<16 | ((g)&0xFF)<<8 | ((b)&0xFF))
#define MIN3(a,b,c) ( (a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c) ( (a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )

//-----------------------------------------------------------------------------
// Name: GFX_Tools constructor
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLTriangleShadow( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3, MT_Float rWidth, MT_Float rStrengthShadow, const GFX_Color& color )
{
    MT_Vector2D vPos( -0.5, -0.5 );
    CreateGLTriangle( vPos1 + vPos, vPos2+ vPos, vPos3+ vPos, color.GetShadow( rStrengthShadow ) );
    
    CreateGLTriangle( vPos1, vPos2, vPos3, color );
    
    GFX_Color colorBorder( 0,0,0,1. );
    CreateGLLine( vPos1, vPos2, rWidth, colorBorder );
    CreateGLLine( vPos2, vPos3, rWidth, colorBorder );
    CreateGLLine( vPos3, vPos1, rWidth, colorBorder );
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools constructor
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------

void GFX_Tools::CreateGLTriangle( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, const MT_Vector2D& vPos3, const GFX_Color& color )
{
    glBindTexture( GL_TEXTURE_2D, 0 );

	color.SetGLColor();

    glBegin( GL_TRIANGLES );
    glVertex2f( vPos1.rX_, vPos1.rY_ );
    glVertex2f( vPos2.rX_, vPos2.rY_ );
    glVertex2f( vPos3.rX_, vPos3.rY_ );
	glEnd();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLTriangle3D
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------

void GFX_Tools::CreateGLTriangle3D( const MT_Vector3D& vPos1, const MT_Vector3D& vPos2, const MT_Vector3D& vPos3, const GFX_Color& color )
{
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glBindTexture( GL_TEXTURE_2D, 0 );

	color.SetGLColor();

    glBegin( GL_TRIANGLES );
    glVertex3f( vPos1.rX_, vPos1.rY_, vPos1.rZ_ );
    glVertex3f( vPos2.rX_, vPos2.rY_, vPos2.rZ_ );
    glVertex3f( vPos3.rX_, vPos3.rY_, vPos3.rZ_);
	glEnd();

    glDisable( GL_CULL_FACE );

}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLLines
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLLines( CT_LinePtrVector& lineVector, MT_Float rWidth, const GFX_Color& color )
{
    glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
	color.SetGLColor();

    glBegin( GL_LINES );
    for( CIT_LinePtrVector itLine = lineVector.begin(); itLine != lineVector.end(); ++itLine )
	{
        const MT_Line& line = **itLine;
        const MT_Vector2D& vPos1 = line.GetPosStart();
		const MT_Vector2D& vPos2 = line.GetPosEnd  ();
		glVertex2f( vPos1.rX_, vPos1.rY_ );
		glVertex2f( vPos2.rX_, vPos2.rY_ );
	}		
    glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLLines
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLLines( CT_PointVector& pointVector, MT_Float rWidth, const GFX_Color& color )
{
    glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
	color.SetGLColor();

    glBegin( GL_LINE_STRIP );

    for( CIT_PointVector itPoint = pointVector.begin(); itPoint != pointVector.end(); ++itPoint )
	{
        const MT_Vector2D& vPos = *itPoint;
        glVertex2f( vPos.rX_, vPos.rY_ );
	}		

    glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLLine
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLLine( const MT_Line& line, MT_Float rWidth, const GFX_Color& color )
{
    glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
	color.SetGLColor();

    const MT_Vector2D& vPos1 = line.GetPosStart();
	const MT_Vector2D& vPos2 = line.GetPosEnd  ();

    glBegin( GL_LINES );
        glVertex2f( vPos1.rX_, vPos1.rY_ );
    	glVertex2f( vPos2.rX_, vPos2.rY_ );
	glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLLine
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLLine( const MT_Vector2D& vPos1, const MT_Vector2D& vPos2, MT_Float rWidth, const GFX_Color& color )
{
    glLineWidth( rWidth );

    color.SetGLColor();

    glBegin( GL_LINES );
        glVertex2f( vPos1.rX_, vPos1.rY_ );
        glVertex2f( vPos2.rX_, vPos2.rY_ );
	glEnd();

}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLLine
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLLine3D( const MT_Vector3D& vPos1, const MT_Vector3D& vPos2, MT_Float rWidth, const GFX_Color& color )
{
    glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
	color.SetGLColor();

    glBegin( GL_LINES );
        glVertex3f( vPos1.rX_, vPos1.rY_, vPos1.rZ_ );
        glVertex3f( vPos2.rX_, vPos2.rY_, vPos2.rZ_ );
	glEnd();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateTrianglesGL
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLTriangles( CT_TrianglePtrVector& vListTriangles, MT_Float rWidth, const GFX_Color& color )
{
	glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );

    color.SetGLColor();

    glBegin( GL_TRIANGLES );
    for( CIT_TrianglePtrVector itTriangle = vListTriangles.begin(); itTriangle != vListTriangles.end(); ++itTriangle )
    {
        const MT_Triangle& triangle = **itTriangle;

		const MT_Vector2D& vPos1 = triangle.GetPos1();
		const MT_Vector2D& vPos2 = triangle.GetPos2();
		const MT_Vector2D& vPos3 = triangle.GetPos3();

        glVertex2f( vPos1.rX_, vPos1.rY_ );
        glVertex2f( vPos2.rX_, vPos2.rY_ );
        glVertex2f( vPos3.rX_, vPos3.rY_ );
	}
    glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLTrianglesLines
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLTrianglesLines( CT_TrianglePtrVector& triangleVector, MT_Float rWidth, const GFX_Color& color )
{
	glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );

	color.SetGLColor();

    for( CIT_TrianglePtrVector itTriangle = triangleVector.begin(); itTriangle != triangleVector.end(); ++itTriangle )
    {
        const MT_Triangle& triangle = **itTriangle;

		const MT_Vector2D& vPos1 = triangle.GetPos1();
		const MT_Vector2D& vPos2 = triangle.GetPos2();
		const MT_Vector2D& vPos3 = triangle.GetPos3();

        glBegin( GL_LINE_STRIP );
            glVertex2f( vPos1.rX_, vPos1.rY_ );
            glVertex2f( vPos2.rX_, vPos2.rY_ );
            glVertex2f( vPos3.rX_, vPos3.rY_ );
		glEnd();
	}

}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLRect
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLRect( const MT_Rect& rect, MT_Float rWidth, const GFX_Color& color )
{
	glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
    color.SetGLColor();

	glBegin( GL_LINE_STRIP );
	    glVertex2f( rect.GetLeft(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetBottom() );
	    glVertex2f( rect.GetLeft(), rect.GetBottom() );
	    glVertex2f( rect.GetLeft(), rect.GetTop() );
	glEnd();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLRect
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLRectPoly( const MT_Rect& rect, const GFX_Color& color )
{
    color.SetGLColor();
    glBindTexture( GL_TEXTURE_2D, 0 );

	glBegin( GL_QUADS );
	    glVertex2f( rect.GetLeft(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetBottom() );
	    glVertex2f( rect.GetLeft(), rect.GetBottom() );
	glEnd();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::GetColorForTeam
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
GFX_Color GFX_Tools::GetColorForTeam( int nTeamId )
{
    switch( nTeamId )
    {
        case 0: return GFX_Color ( 255, 000, 255 );    
        case 1: return GFX_Color ( 000, 000, 255 );
        case 2: return GFX_Color ( 255, 000, 000 );        
        case 3: return GFX_Color ( 000, 255, 255 );
        case 4: return GFX_Color ( 255, 255, 000 );
        case 5: return GFX_Color ( 100, 100, 100 );
        default:return GFX_Color ( 255, 100, 255 );
    }
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLCross
// Created: FBD 02-11-25
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLCross( const MT_Vector2D& vCenter, MT_Float rRadius, MT_Float rWidth, const GFX_Color& color )
{
	glLineWidth( rWidth );
    glBindTexture( GL_TEXTURE_2D, 0 );
    color.SetGLColor();

    glBegin( GL_LINES );
        glVertex2f( vCenter.rX_ - rRadius, vCenter.rY_ - rRadius );
        glVertex2f( vCenter.rX_ + rRadius, vCenter.rY_ + rRadius );
        glVertex2f( vCenter.rX_ + rRadius, vCenter.rY_ - rRadius );
        glVertex2f( vCenter.rX_ - rRadius, vCenter.rY_ + rRadius );
    glEnd();
}
//-----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLButton
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLButton( const MT_Rect& rect, const GFX_Color& color, MT_Float rSizeBorder )
{
    color.SetGLColor();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBegin( GL_QUADS );
	    glVertex2f( rect.GetLeft() , rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetBottom() );
	    glVertex2f( rect.GetLeft(), rect.GetBottom() );

    GFX_Color black( 16, 16, 16, color.GetAlpha() );
    black.SetGLColor();

	    glVertex2f( rect.GetLeft(), rect.GetTop() );
	    glVertex2f( rect.GetLeft() + rSizeBorder, rect.GetTop() );
	    glVertex2f( rect.GetLeft() + rSizeBorder, rect.GetBottom() );
	    glVertex2f( rect.GetLeft(), rect.GetBottom() );

        glVertex2f( rect.GetLeft(), rect.GetTop() );
        glVertex2f( rect.GetRight() - rSizeBorder, rect.GetTop() );
        glVertex2f( rect.GetRight() - rSizeBorder, rect.GetTop() - rSizeBorder );
        glVertex2f( rect.GetLeft(), rect.GetTop() - rSizeBorder );
    

    GFX_Color white( 128, 128, 128, color.GetAlpha() );
    white.SetGLColor();

	    glVertex2f( rect.GetRight() - rSizeBorder, rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetTop() );
	    glVertex2f( rect.GetRight(), rect.GetBottom() );
	    glVertex2f( rect.GetRight() - rSizeBorder, rect.GetBottom() );

        glVertex2f( rect.GetLeft() + rSizeBorder, rect.GetBottom() + rSizeBorder );
        glVertex2f( rect.GetRight(), rect.GetBottom() + rSizeBorder );
        glVertex2f( rect.GetRight(), rect.GetBottom() );
        glVertex2f( rect.GetLeft() + rSizeBorder, rect.GetBottom() );
	glEnd();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::ClipBorders
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
void GFX_Tools::ClipBordersRect( const MT_Rect& rect )
{
    MT_Rect rectLeft( rect.GetLeft() - rect.GetWidth(), rect.GetTop(), rect.GetLeft(), rect.GetBottom() );
    MT_Rect rectRight( rect.GetRight(), rect.GetTop(), rect.GetRight() + rect.GetWidth(), rect.GetBottom() );

    MT_Rect rectTop( rect.GetLeft() - rect.GetWidth(), rect.GetTop() + rect.GetHeight(), rect.GetRight() + rect.GetWidth(), rect.GetTop() );
    MT_Rect rectBottom( rect.GetLeft() - rect.GetWidth(), rect.GetBottom() - rect.GetHeight(), rect.GetRight() + rect.GetWidth(), rect.GetBottom() );

    GFX_Tools::CreateGLRectPoly( rectLeft, GFX_Color( 0., 0., 0. ) );
    GFX_Tools::CreateGLRectPoly( rectRight, GFX_Color( 0., 0., 0. ) );
    GFX_Tools::CreateGLRectPoly( rectTop, GFX_Color( 0., 0., 0. ) );
    GFX_Tools::CreateGLRectPoly( rectBottom, GFX_Color( 0., 0., 0. ) );
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::DrawCircle
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLCircle( const MT_Vector2D& center, MT_Float radius, const GFX_Color& color )
{
    CreateGLArc( center, radius, 0, 3.14159265358979323 * 2., color );
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::DrawArc
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLArc( const MT_Vector2D& center, MT_Float radius, MT_Float beginAngle, MT_Float endAngle, const GFX_Color& color )
{
    glLineWidth( 0.5 );
    color.SetGLColor( );
    glBindTexture( GL_TEXTURE_2D, 0 );

    if ( beginAngle > endAngle )
        std::swap( beginAngle, endAngle );

    glBegin( GL_LINE_STRIP );
    for ( MT_Float angle = beginAngle ; angle <= endAngle; angle += 0.01f )
    {
        MT_Float X, Y;
        X = center.rX_ + ( radius * (MT_Float)cos( (double)angle ) );
        Y = center.rY_ + ( radius * (MT_Float)sin( (double)angle ) );
        glVertex2d( X, Y );
    }
    glVertex2d( center.rX_ + ( radius * (MT_Float)cos( (double)endAngle ) ), center.rY_ + ( radius * (MT_Float)sin( (double)endAngle ) ) );
    glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::DrawArrow
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
void GFX_Tools::CreateGLArrow( const MT_Vector2D& src, const MT_Vector2D& dest, const GFX_Color& color, MT_Float rSize, MT_Float rSizeArrow )
{
    if ( src == dest )
        return; 

    MT_Vector2D tmp( (src - dest).Normalized() * rSizeArrow );
    MT_Vector2D tmp2( 2. * tmp );

    color.SetGLColor();
    
    std::swap( tmp.rX_, tmp.rY_ );
    tmp.rX_ = -tmp.rX_;

    glLineWidth( rSize );

    glBegin( GL_LINES );
	glVertex2f( src.rX_, src.rY_ );
	glVertex2f( dest.rX_, dest.rY_ );
	glEnd();

    MT_Vector2D pos ( dest + tmp + tmp2 );

    glBegin( GL_LINES );
	glVertex2f( dest.rX_, dest.rY_ );
	glVertex2f( pos.rX_, pos.rY_ );
	glEnd();

    pos = dest - tmp + tmp2;

    glBegin( GL_LINES );
	glVertex2f( dest.rX_, dest.rY_ );
	glVertex2f( pos.rX_, pos.rY_ );
	glEnd();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::InitializeFonts
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Tools::InitializeFonts()
{
    fontText_.Initialize();
    fontAPP6FR_.Initialize();
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::DeleteFonts
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Tools::DeleteFonts()
{
    fontAPP6FR_.Terminate();
    fontText_.Terminate();
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::Print
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Tools::Print( MT_Vector2D vPos, MT_Float rSize, const GFX_Color& color, bool bCenter, const char *fmt, ...)
{
	char		text[256];		
    va_list		ap;				
	if (fmt == NULL)			
		return;					

    va_start(ap, fmt);			
	    vsprintf(text, fmt, ap);
	va_end(ap);					
    fontText_.Print( vPos, rSize, color, bCenter, text );
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::PrintWithShadows
// Created: FBD 02-12-13
//-----------------------------------------------------------------------------
void GFX_Tools::PrintWithShadows( MT_Vector2D vPos, MT_Float rSize, MT_Float rSizeShadow, MT_Float rStrengthShadow, const GFX_Color& color, bool bCenter, const char *fmt, ...)
{
	char		text[256];								// Holds Our String
    va_list		ap;										// Pointer To List Of Arguments
	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

    va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

    fontText_.PrintLine( vPos, rSize, rSizeShadow, color.GetShadow( rStrengthShadow ), bCenter, text );
    fontText_.Print( vPos, rSize, color, bCenter, text );
}


//-----------------------------------------------------------------------------
// Name: GFX_Tools::IsInsideGLAgent
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
bool GFX_Tools::IsInsideGLAgent( const MT_Vector2D& vPosAgent, MT_Float rSize, const std::string& sUnit, const MT_Vector2D& vPos )
{
    MT_Vector2D vSize = fontAPP6FR_.GetSizeText( sUnit );
    vSize *= rSize;
    MT_Rect rect( vPosAgent.rX_ - vSize.rX_ , vPosAgent.rY_ - vSize.rY_ , vPosAgent.rX_ + vSize.rX_, vPosAgent.rY_ + vSize.rY_ );
    return rect.IsInside( vPos);
}


// -----------------------------------------------------------------------------
// Name: GFX_Tools::CreateGLAgentShadow
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void GFX_Tools::CreateGLAgentShadow( MT_Vector2D vPos, MT_Float rSize, MT_Float rSizeShadow, MT_Float rStrengthShadow, const GFX_Color& color, bool bCenter, const std::string& sUnit, MT_Float rPercentLife , bool bBackground )
{
    if( rPercentLife >= 0 )
    {
        MT_Vector2D vSize = fontAPP6FR_.GetSizeText( sUnit );

        glPushMatrix();
        glTranslatef( vPos.rX_, vPos.rY_, 0. );
        glScalef( rSize, rSize, 1.f );
        if ( bCenter )
		    glTranslatef( -( vSize.rX_ / 2.f ), 0.0, 0.0 );
        MT_Rect rectBackLife( vSize.rX_*0.05, 0, vSize.rX_*0.95, vSize.rY_ );
        
        GFX_Color backgroundColor = GFX_Color( 255, 255, 255, 0.4 );
        // On trace le fond de la life
        if( bBackground)
        {
            backgroundColor = GFX_Color( 255, 255, 0.0, 0.4 );
            CreateGLRectPoly( rectBackLife, backgroundColor  );
        }
        
        // On trace la life
        MT_Float rHeight = ( rectBackLife.GetTop() - rectBackLife.GetBottom() ) * rPercentLife / 100.;
        MT_Rect rectLife( rectBackLife.GetLeft(), rectBackLife.GetBottom(), rectBackLife.GetRight(), rectBackLife.GetBottom() + rHeight );
        CreateGLRectPoly( rectLife, backgroundColor );
        glPopMatrix();
    }

    // On trace la shape
    GFX_Color colorShape( color );
    colorShape.SetAlpha( 0.9 );
    fontAPP6FR_.PrintLine( vPos, rSize, rSizeShadow, colorShape.GetShadow( rStrengthShadow ), bCenter, sUnit.c_str() );
    fontAPP6FR_.Print( vPos, rSize, colorShape, bCenter, sUnit.c_str() );
}




//-----------------------------------------------------------------------------
// Name: GFX_Tools::ColorScaleHSL
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
ulong GFX_Tools::ColorScaleHSL(	const ulong Col1, const ulong Col2, const MT_Float Ratio )
{
	MT_Float H1, H2, S1, S2, L1, L2;
	int rgb;
	
	if (Ratio<=0) 
        return Col1;	// Ratio parameter must be between 0 and 1
	else 
    if (Ratio>=1) 
        return Col2;

	RGBtoHLS( Col1, &H1, &L1, &S1);
	RGBtoHLS( Col2, &H2, &L2, &S2);
	return rgb = HLStoRGB( H1+(H2-H1)*Ratio, L1+(L2-L1)*Ratio, S1+(S2-S1)*Ratio );
}



//-----------------------------------------------------------------------------
// Name: GFX_Tools::RGBtoHLS
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
void GFX_Tools::RGBtoHLS( const ulong rgb, MT_Float* H, MT_Float* L, MT_Float *S )
{
	MT_Float delta;
	MT_Float r = (MT_Float)((rgb>>16)&0xFF)/255;
	MT_Float g = (MT_Float)((rgb>> 8)&0xFF)/255;
	MT_Float b = (MT_Float)((rgb    )&0xFF)/255;
	MT_Float cmax = MAX3(r,g,b);
	MT_Float cmin = MIN3(r,g,b);
	*L=(cmax+cmin)/2.0;

	if(cmax==cmin) 
        *S = *H = 0; // it's really undefined
	else 
    {
		if(*L < 0.5)	
            *S = (cmax-cmin)/(cmax+cmin);
		else			
            *S = (cmax-cmin)/(2.0-cmax-cmin);
		delta = cmax - cmin;
		if (r==cmax) *H = (g-b)/delta;
		else 
	    if(g==cmax) 
            *H = 2.0 +(b-r)/delta;
	    else        
            *H = 4.0+(r-g)/delta;
	  	*H /= 6.0;
	  	if (*H < 0.0) 
            *H += 1;
	}
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::HLStoRGB
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
ulong GFX_Tools::HLStoRGB(const MT_Float H, const MT_Float L, const MT_Float S )
{
	MT_Float r,g,b;
	MT_Float m1, m2;

	if (S==0) 
        r=g=b=L;
	else 
    {
		if (L <=0.5) 
            m2 = L*(1.0+S);
		else         
            m2 = L+S-L*S;
		m1 = 2.0*L-m2;
		r = HueToRGB(m1,m2,H+1.0/3.0);
		g = HueToRGB(m1,m2,H);
		b = HueToRGB(m1,m2,H-1.0/3.0);
	}
    return DefMakeRGB((uchar)(r*255),(uchar)(g*255),(uchar)(b*255));
}

//-----------------------------------------------------------------------------
// Name: GFX_Tools::HueToRGB
// Created: FBD 03-02-13
//-----------------------------------------------------------------------------
MT_Float GFX_Tools::HueToRGB( const MT_Float m1, const MT_Float m2, MT_Float h )
{
	if (h<0) 
        h+=1.0;
	if (h>1) 
        h-=1.0;
	if (6.0*h < 1  ) 
        return (m1+(m2-m1)*h*6.0);
	if (2.0*h < 1  ) 
        return m2;
	if (3.0*h < 2.0) 
        return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
	return m1;
}
