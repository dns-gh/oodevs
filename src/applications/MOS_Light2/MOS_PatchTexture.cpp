//*****************************************************************************
// Name   : MOS_PatchTexture.cpp
// Created: FBD 02-02-27
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_PatchTexture.h"

#include "MOS_App.h"

#ifndef MOS_USE_INLINE
#   include "MOS_PatchTexture.inl"
#endif


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture constructor
// Created: FBD 02-02-27
//-----------------------------------------------------------------------------
MOS_PatchTexture::MOS_PatchTexture( const MT_Rect& rect )
    : rect_              ( rect )
    , nGLPatchAltitude_  ( 0 )
    , nGLPatchCouverture_( 0 )
{
    assert( rect_.GetWidth() != 0 && rect.GetHeight() != 0 );
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_PatchTexture destructor
// Created: FBD 02-02-27
//-----------------------------------------------------------------------------
MOS_PatchTexture::~MOS_PatchTexture()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::GenerateCouverture
//
// Creates a texture with the locations of the towns and forests.
// Kept for eventual future use.
//
// Created: FBD 03-04-11
//-----------------------------------------------------------------------------
void MOS_PatchTexture::GenerateCouverture()
{
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
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );

	glEndList();
    delete pImage_;
    free( pBuffer );
}


//-----------------------------------------------------------------------------
// Name: MOS_PatchTexture::Initialize
// Created: FBD 02-11-27
//-----------------------------------------------------------------------------
void MOS_PatchTexture::Initialize()
{
    GenerateAltitude();
}




