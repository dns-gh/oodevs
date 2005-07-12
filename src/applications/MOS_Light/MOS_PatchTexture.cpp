//*****************************************************************************
// Name   : MOS_PatchTexture.cpp
// Created: FBD 02-02-27
//*****************************************************************************

#include "MOS_pch.h"

#include "MOS_PatchTexture.h"

#include "MOS_World.h"
#include "MOS_GLWidget.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"

#include <QImage.h>

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture constructor
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
MOS_PatchTexture::MOS_PatchTexture    ( )
    : rect_             ()
    , nIdxPatchX_       ( 0 )
    , nIdxPatchY_       ( 0 )
    , pPatchUp_         ( 0 )
	, pPatchRight_      ( 0 )
	, pPatchDown_       ( 0 )
	, pPatchLeft_       ( 0 )
    , rHandlePatch_     ( 0.)
    , nFrame_           ( 0 )
{
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture constructor
// Created: FBD 02-02-27
//-----------------------------------------------------------------------------
MOS_PatchTexture::MOS_PatchTexture( const MT_Rect& rect, uint nIdxPatchX, uint nIdxPatchY )
    : rect_       ( rect )
    , nIdxPatchX_ ( nIdxPatchX )
    , nIdxPatchY_ ( nIdxPatchY )
    , pPatchUp_   ( 0 )
	, pPatchRight_( 0 )
	, pPatchDown_ ( 0 )
	, pPatchLeft_ ( 0 )
    , rHandlePatch_( rect.GetCenter().rY_ * MOS_App::GetApp().GetWorld().GetRect().GetWidth() + rect.GetCenter().rX_ )
    , nFrame_             ( 0 )
{
}



//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GenerateCouverture
// Created: FBD 03-04-11
//-----------------------------------------------------------------------------
void MOS_PatchTexture::GenerateCouverture()
{
//    glRect_ = glGenLists( 1 );
//    glNewList( glRect_, GL_COMPILE );
//    GFX_Tools::CreateGLRect( rect_, 1.0, GFX_Color( 255, 255, 255, 0.1 ) );
//   glEndList();


    int nRawWidth = rect_.GetWidth();
    int nRawHeight = rect_.GetHeight();
	if ( nRawWidth == 0 || nRawHeight == 0 )
		return;

    nRawWidth += 1;
    nRawHeight += 1;

	int nRealSizeX = nRawWidth;
	int nRealSizeY = nRawHeight;

	uint i;
	for ( i = 31 ; i >= 0 ; --i )
	{
		if ( nRawWidth & ( 1 << i ) )
		{
			nRealSizeX = 1 << ( i +1 );
			break;
		}
	}

	for ( i = 31 ; i >= 0 ; --i )
	{
		if ( nRawHeight & ( 1 << i ) )
		{
			nRealSizeY = 1 << ( i +1 );
			break;
		}
		
	}



    #define VAL_WHITE 0xff80ff
    #define VAL_BLACK 0x000000

    uint* pBuffer = (uint*)malloc( ( ( nRawHeight ) * ( nRawWidth ) ) * sizeof( uint ) );
    int nPosY = 0;
    for ( MT_Float rY = rect_.GetBottom() ; rY < rect_.GetTop() ; rY += 1. )
    {
        int nPosX = 0;
        for ( MT_Float rX = rect_.GetLeft() ; rX < rect_.GetRight() ; rX += 1. )
        {
            MOS_RawVisionData::envBits terrain = MOS_App::GetApp().GetRawVisionData()( rX, rY ).e;
            uint nColor = 0x0;
            if ( terrain & MOS_RawVisionData::E_VisionObject::eVisionForest )
                nColor |= 0x00ff00;
            if ( terrain & MOS_RawVisionData::E_VisionObject::eVisionUrban )
                nColor |= 0x0ffff00;
           
            
            pBuffer[ nPosY * nRawWidth + nPosX ] = nColor;
            ++nPosX;

        }
        ++nPosY;
    
    }


    QImage imageTmp( (uchar*)pBuffer, nRawWidth , nRawHeight , 32,(QRgb*)NULL, 0, QImage::LittleEndian );
    QSize size( nRealSizeX, nRealSizeY );
    QImage* pImage_ = new QImage( imageTmp.scale( size ) ); 
	
    nGLPatchCouverture_ = glGenLists(1);
    glNewList( nGLPatchCouverture_, GL_COMPILE);
    glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable   ( GL_BLEND );

    
    glNormal3f( 0.0, 0.0, 1.0 );
	glColor4f( 1.0, 1.0, 1.0, 0.5 );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, pImage_->width(), pImage_->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage_->bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glBegin( GL_QUADS );

    MT_Float rRatioX = (MT_Float)nRealSizeX / rect_.GetWidth();
    MT_Float rRatioY = (MT_Float)nRealSizeY / rect_.GetHeight();

    MT_Float rPrecX1 = ( rect_.GetLeft() - (int)rect_.GetLeft() ) * rRatioX;
    MT_Float rPrecY1 = ( 1. - ( rect_.GetTop() - (int)rect_.GetTop() ) ) * rRatioY;

    MT_Vector2D vPos1( rPrecX1 / ( (MT_Float)( nRealSizeX ) ), rPrecY1 / ( (MT_Float)( nRealSizeY ) ) );
    vPos1.rY_ = 1. - vPos1.rY_;
    
    MT_Float rPrecX2 = ( 1. - ( rect_.GetRight() - (int)rect_.GetRight() ) ) * rRatioX;
    MT_Float rPrecY2 = ( rect_.GetBottom() - (int)rect_.GetBottom() ) * rRatioY;

    MT_Vector2D vPos2( rPrecX2 / ( (MT_Float)( nRealSizeX ) ), rPrecY2 / ( (MT_Float)( nRealSizeY ) ) );
    vPos2.rX_ = 1. - vPos2.rX_;
    
    MT_Float rZ_ = 0.;
	glTexCoord2f( vPos1.rX_, vPos1.rY_ );                                   // 0,1
	glVertex3f( rect_.GetLeft(), rect_.GetTop(), rZ_ );
	glTexCoord2f( vPos2.rX_, vPos1.rY_ );                                   // 1,1
	glVertex3f( rect_.GetRight(), rect_.GetTop(), rZ_ );
	glTexCoord2f( vPos2.rX_, vPos2.rY_ );                                   // 1,0
	glVertex3f( rect_.GetRight(), rect_.GetBottom() , rZ_ );
	glTexCoord2f( vPos1.rX_, vPos2.rY_ );                                   // 0,0
	glVertex3f( rect_.GetLeft(), rect_.GetBottom(), rZ_ );

    glEnd();

	glDisable( GL_BLEND );

    glEnable( GL_DEPTH_TEST );

	glEndList();
    delete pImage_;
    free( pBuffer );
    
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GenerateAltitude
// Created: FBD 03-04-11
//-----------------------------------------------------------------------------
void MOS_PatchTexture::GenerateAltitude()
{
    int nRawWidth = rect_.GetWidth();
    int nRawHeight = rect_.GetHeight();

	int nRealSizeX = nRawWidth;
	int nRealSizeY = nRawHeight;
	if ( nRawWidth == 0 || nRawHeight == 0 )
		return;

    nRawWidth += 1;
    nRawHeight += 1;


	uint i;
	for ( i = 31 ; i >= 0 ; --i )
	{
		if ( nRawWidth & ( 1 << i ) )
		{
			nRealSizeX = 1 << ( i +1 );
			break;
		}
	}

	for ( i = 31 ; i >= 0 ; --i )
	{
		if ( nRawHeight & ( 1 << i ) )
		{
			nRealSizeY = 1 << ( i +1 );
			break;
		}
		
	}


    MT_Float rMinAltitude = MOS_App::GetApp().GetRawVisionData().GetMinAltitude();
    MT_Float rMaxAltitude = MOS_App::GetApp().GetRawVisionData().GetMaxAltitude();
    #define VAL_WHITE 0xff80ff
    #define VAL_BLACK 0x000000

    uint* pBuffer = (uint*)malloc( ( ( nRawHeight ) * ( nRawWidth  ) ) * sizeof( uint ) );
    int nPosY = 0;
    for ( MT_Float rY = rect_.GetBottom() ; rY < rect_.GetTop() ; rY += 1. )
    {
        int nPosX = 0;
        for ( MT_Float rX = rect_.GetLeft() ; rX < rect_.GetRight() ; rX += 1. )
        {
            MT_Float rAltitude =MOS_App::GetApp().GetRawVisionData().GetHeight( MT_Vector2D( rX, rY ) );
            uint nColor = GFX_Tools::ColorScaleHSL( VAL_BLACK, VAL_WHITE, (rAltitude - rMinAltitude ) / ( rMaxAltitude - rMinAltitude ) );
            pBuffer[ nPosY * nRawWidth + nPosX ] = nColor;
            ++nPosX;

        }
        ++nPosY;
    
    }


    QImage imageTmp( (uchar*)pBuffer, nRawWidth, nRawHeight , 32,(QRgb*)NULL, 0, QImage::LittleEndian );
    QSize size( nRealSizeX, nRealSizeY );
    QImage* pImage_ = new QImage( imageTmp.scale( size ) ); 
	
    nGLPatchAltitude_ = glGenLists(1);
    glNewList( nGLPatchAltitude_, GL_COMPILE);
    glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable   ( GL_BLEND );

    
    glNormal3f( 0.0, 0.0, 1.0 );
	glColor4f( 1.0f, 1.0f, 1.0f, 0.7f );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, pImage_->width(), pImage_->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage_->bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glBegin( GL_QUADS );
    MT_Float rRatioX = (MT_Float)nRealSizeX / rect_.GetWidth();
    MT_Float rRatioY = (MT_Float)nRealSizeY / rect_.GetHeight();

    MT_Float rPrecX1 = ( rect_.GetLeft() - (int)rect_.GetLeft() ) * rRatioX;
    MT_Float rPrecY1 = ( 1. - ( rect_.GetTop() - (int)rect_.GetTop() ) ) * rRatioY;

    MT_Vector2D vPos1( rPrecX1 / ( (MT_Float)( nRealSizeX ) ), rPrecY1 / ( (MT_Float)( nRealSizeY ) ) );
    vPos1.rY_ = 1. - vPos1.rY_;
    
    MT_Float rPrecX2 = ( 1. - ( rect_.GetRight() - (int)rect_.GetRight() ) ) * rRatioX;
    MT_Float rPrecY2 = ( rect_.GetBottom() - (int)rect_.GetBottom() ) * rRatioY;

    MT_Vector2D vPos2( rPrecX2 / ( (MT_Float)( nRealSizeX ) ), rPrecY2 / ( (MT_Float)( nRealSizeY ) ) );
    vPos2.rX_ = 1. - vPos2.rX_;

    MT_Float rZ_ = 0.;
	glTexCoord2f( vPos1.rX_, vPos1.rY_ );                                   // 0,1
	glVertex3f( rect_.GetLeft(), rect_.GetTop(), rZ_ );
	glTexCoord2f( vPos2.rX_, vPos1.rY_ );                                   // 1,1
	glVertex3f( rect_.GetRight(), rect_.GetTop(), rZ_ );
	glTexCoord2f( vPos2.rX_, vPos2.rY_ );                                   // 1,0
	glVertex3f( rect_.GetRight(), rect_.GetBottom() , rZ_ );
	glTexCoord2f( vPos1.rX_, vPos2.rY_ );                                   // 0,0
	glVertex3f( rect_.GetLeft(), rect_.GetBottom(), rZ_ );

    glEnd();

	glDisable( GL_BLEND );

    glEnable( GL_DEPTH_TEST );

	glEndList();
    delete pImage_;
    free( pBuffer );

}



//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::InitializeGFX
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
void MOS_PatchTexture::InitializeGFX()
{
    GenerateAltitude();
//    GenerateCouverture();
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::TerminateGFX
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
void MOS_PatchTexture::TerminateGFX()
{
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture::IsOnScreen
// Created: FBD 02-07-04
//-----------------------------------------------------------------------------
bool MOS_PatchTexture::IsOnScreen()
{
	GLdouble	modelMatrix[16], projMatrix[16];
	GLint		viewport[4];

    glPushMatrix ();

	glGetDoublev ( GL_MODELVIEW_MATRIX, modelMatrix ); 
	glGetDoublev ( GL_PROJECTION_MATRIX, projMatrix ); 
	glGetIntegerv( GL_VIEWPORT, viewport); 

	GLdouble rZ;
	GLdouble rX1PatchdProj, rY1PatchdProj; 
	GLdouble rX2PatchdProj, rY2PatchdProj;

	gluProject( rect_.GetLeft (), rect_.GetBottom(), 0.0, modelMatrix, projMatrix, viewport, &rX1PatchdProj, &rY1PatchdProj, &rZ );
	gluProject( rect_.GetRight(), rect_.GetTop   (), 0.0, modelMatrix, projMatrix, viewport, &rX2PatchdProj, &rY2PatchdProj, &rZ );
	
	glPopMatrix();

    MOS_GLWidget& glWidget = MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget();

	if ( rX2PatchdProj < 0 || rX1PatchdProj > glWidget.width())
        return false;
	
	if ( rY2PatchdProj < 0 || rY1PatchdProj > glWidget.height() )
        return false;

    return true;
}

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_PatchTexture::Draw( MT_Float /*rDistPercent*/ )
{
    if( !IsOnScreen() )
        return;

//    if ( rDistPercent < 70. )
    glCallList( nGLPatchCouverture_ );
    glCallList( nGLPatchAltitude_ );
//  glCallList( glRect_ );

}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture destructor
// Created: FBD 02-02-27
//-----------------------------------------------------------------------------
MOS_PatchTexture::~MOS_PatchTexture()
{
}

//=============================================================================
// PATCHS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::LinkToNeighbors
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
void MOS_PatchTexture::LinkToNeighbors()
{
    MOS_World& world = MOS_App::GetApp().GetWorld();
    
	pPatchUp_    = world.GetPatchTextureFromIdx( nIdxPatchX_    , nIdxPatchY_ - 1 );
	pPatchRight_ = world.GetPatchTextureFromIdx( nIdxPatchX_ + 1, nIdxPatchY_     );
	pPatchDown_  = world.GetPatchTextureFromIdx( nIdxPatchX_    , nIdxPatchY_ + 1 );
	pPatchLeft_  = world.GetPatchTextureFromIdx( nIdxPatchX_ - 1, nIdxPatchY_     );
}

