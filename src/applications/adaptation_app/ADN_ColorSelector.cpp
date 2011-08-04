// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ColorSelector.h"
#include "moc_ADN_ColorSelector.cpp"
#include "ADN_App.h"
#include "ADN_Connector_String.h"
#include "ADN_Enums.h"
#include "ADN_MainWindow.h"

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector constructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ColorSelector::ADN_ColorSelector( QWidget* parent )
    : QPushButton( parent )
{
    setMinimumSize( 25, 25 );
    setMaximumWidth( 3 * height() );
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
    pConnector_ = new ADN_Connector_String< ADN_ColorSelector >( this );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector destructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ColorSelector::~ADN_ColorSelector()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::setEnabled
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QPushButton::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QPushButton::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::setText
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::setText( const QString& string )
{
    if( string.length() >= 6 )
        color_.setNamedColor( "#" + string.right( 6 ) );
    else
        color_.setNamedColor( "#000000" );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::UpdateEnableState
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String< ADN_ColorSelector >* >( pConnector_ )->IsConnected() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::drawButton
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::drawButton( QPainter* painter )
{
    this->drawButton(painter);
    painter->fillRect( 4, 4, width() - 8, height() - 8, QBrush( QColor( color_ ) ) );
    painter->drawRect( 4, 4, width() - 8, height() - 8 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::OnClick
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::OnClick()
{
    QColor color = QColorDialog::getColor( color, this );
    if( color.isValid() )
    {
        color_ = color;
        repaint();
        static_cast< ADN_Connector_String< ADN_ColorSelector >* >( pConnector_ )->SetDataChanged( "0x" + color_.name().right( 6 ) );
    }
}
