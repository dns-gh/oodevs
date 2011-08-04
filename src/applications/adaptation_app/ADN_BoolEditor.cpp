// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_BoolEditor.cpp $
// $Author: Ape $
// $Modtime: 28/01/05 11:40 $
// $Revision: 2 $
// $Workfile: ADN_BoolEditor.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_BoolEditor.h"
#include "moc_ADN_BoolEditor.cpp"
#include "res/checked_box.xpm"
#include "res/empty_box.xpm"

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor constructor
/** @param  pParent
    @param  bState
*/
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
ADN_BoolEditor::ADN_BoolEditor( QWidget* pParent, bool bState )
: Q3Frame      ( pParent )
, bState_     ( bState )
{
    setFrameStyle    ( Panel + Plain  );
    setBackgroundMode( Qt::PaletteBase );
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor destructor
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
ADN_BoolEditor::~ADN_BoolEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::mousePressEvent
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
void ADN_BoolEditor::mousePressEvent( QMouseEvent*  )
{
    SetState( !bState_ );
    emit StateChanged( bState_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::CheckedBoxPixmap
/** @return
*/
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
const QPixmap& ADN_BoolEditor::CheckedBoxPixmap()
{
    static const QPixmap pixmap_ = QPixmap( checked_box_xpm );
    return pixmap_;
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::EmptyBoxPixmap
/** @return
*/
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
const QPixmap& ADN_BoolEditor::EmptyBoxPixmap()
{
    static const QPixmap pixmap_ = QPixmap( empty_box_xpm );
    return pixmap_;
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::paintEvent
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
void ADN_BoolEditor::paintEvent( QPaintEvent * pEvent )
{
    Q3Frame::paintEvent( pEvent );
    QPainter painter( this );

    const QRect& rect = pEvent->rect();

    const QPixmap* pPixmap = 0;
    if( bState_ )
    {
        pPixmap = &CheckedBoxPixmap();
    }
    else
    {
        pPixmap = &EmptyBoxPixmap();
    }


    int dx = ( rect.width() - pPixmap->width() ) / 2;
    if( dx < 0 )
        dx = 0;

    int dy = ( rect.height() - pPixmap->height() ) / 2;
    if( dy < 0 )
        dy = 0;

    painter.drawPixmap( dx, dy, *pPixmap );
}

// -----------------------------------------------------------------------------
// Name: ADN_BoolEditor::SetState
// Created: AGN 2004-03-11
// -----------------------------------------------------------------------------
void ADN_BoolEditor::SetState( bool bState )
{
    bState_ = bState;
    update();
}
