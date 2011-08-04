// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ImageViewer.cpp $
// $Author: Ape $
// $Modtime: 11/01/05 10:11 $
// $Revision: 2 $
// $Workfile: ADN_ImageViewer.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ImageViewer.h"

#include "ADN_Wizard_Image.h"

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer constructor
/** @param  fileName
    @param  pParent
    @param  name
*/
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
ADN_ImageViewer::ADN_ImageViewer( const char* fileName, bool bFromRessouce, QWidget* pParent /* = 0*/, const char* name )
    : QWidget( pParent, name )
    , alloc_context_( 0 )
{
    LoadImage( fileName, bFromRessouce );
}

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer destructor
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
ADN_ImageViewer::~ADN_ImageViewer()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer::LoadImage
/** @param  fileName
    @return
*/
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
bool ADN_ImageViewer::LoadImage( const char *fileName, bool bFromRessource )
{
    bool bOk = false;
    if( ! bFromRessource )
        bOk = image_.load( fileName );
    else
    {
        bOk = image_.loadFromData( qembed_findData( fileName ), "BMP" );
    }

    if( bOk )
        return ReconvertImage();
    else
        return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer::ReconvertImage
/** @return
*/
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
bool ADN_ImageViewer::ReconvertImage()
{
    bool success = false;

    if( image_.isNull() ) return false;

    // Clear the image to hide flickering palette
    QPainter painter(this);
    painter.eraseRect( 0, 0, width(), height());

    if( pm_.convertFromImage( image_ ) )
    {
        pmScaled_ = QPixmap();
        Scale();
        resize( width(), height() );
        success = TRUE;             // load successful
    }
    else
    {
        pm_.resize(0,0);                // couldn't load image
    }

    return success;             // TRUE if loaded OK
}

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer::void  paintEvent
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
void ADN_ImageViewer::paintEvent( QPaintEvent * e)
{
    if( pm_.size() != QSize( 0, 0 ) )
    {       // is an image loaded?
        QPainter painter(this);
        painter.setClipRect(e->rect());
        painter.drawPixmap(0, 0, pmScaled_);
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer::void  resizeEvent
/** @param  pEvent
*/
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
void ADN_ImageViewer::resizeEvent( QResizeEvent * )
{
    if( pm_.size() == QSize( 0, 0 ) )      // we couldn't load the image
        return;

    int h = height();
    if( width() != pmScaled_.width()
        || h != pmScaled_.height())
    {                       // if new size,
        Scale();                // scale pmScaled to window
    }

    if( image_.hasAlphaBuffer() )
        erase();
}


// -----------------------------------------------------------------------------
// Name: ADN_ImageViewer::Scale
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
void ADN_ImageViewer::Scale()
{
    int h = height();

    if( image_.isNull() )
        return;

    if( width() == pm_.width()
        && h == pm_.height() )
    {                       // no need to scale if widget
        pmScaled_ = pm_;                // size equals pixmap size
    }
    else
    {
        QMatrix m;             // transformation matrix
        m.scale(((double)width())/pm_.width(),// define scale factors
            ((double)h)/pm_.height());
        pmScaled_ = pm_.xForm( m );     // create scaled pixmap
    }
}
