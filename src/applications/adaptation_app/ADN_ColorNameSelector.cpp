// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ColorNameSelector.h"
#include "moc_ADN_ColorNameSelector.cpp"
#include "ADN_App.h"
#include "ADN_Connector_String.h"
#include "ADN_Enums.h"
#include "ADN_MainWindow.h"

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector constructor
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
ADN_ColorNameSelector::ADN_ColorNameSelector( QWidget* parent )
    : QPushButton( parent )
{
    setMinimumSize( 25, 25 );
    setMaximumWidth( 3 * height() );
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
    pConnector_ = new ADN_Connector_String< ADN_ColorNameSelector >( this );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector destructor
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
ADN_ColorNameSelector::~ADN_ColorNameSelector()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector::setEnabled
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
void ADN_ColorNameSelector::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QPushButton::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QPushButton::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector::setText
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
void ADN_ColorNameSelector::setText( const QString& string )
{
    color_.setNamedColor( string );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector::UpdateEnableState
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
void ADN_ColorNameSelector::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String< ADN_ColorNameSelector >* >( pConnector_ )->IsConnected() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector::paintEvent
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
void ADN_ColorNameSelector::paintEvent( QPaintEvent *e )
{
    QPushButton::paintEvent( e );
    QPainter painter( this );
    QRect rc( 6, 6, painter.window().width() - 12, painter.window().height() - 12 );
    painter.setPen( Qt::darkBlue );
    painter.fillRect( rc, QBrush( QColor( color_ ) ) );
    painter.drawRect( rc );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorNameSelector::OnClick
// Created: LGY 2011-09-22
// -----------------------------------------------------------------------------
void ADN_ColorNameSelector::OnClick()
{
    QColor color = QColorDialog::getColor( color, this, tr( "Select color" ) );
    if( color.isValid() )
    {
        color_ = color;
        repaint();
        static_cast< ADN_Connector_String< ADN_ColorNameSelector >* >( pConnector_ )->SetDataChanged( color_.name() );
    }
}
