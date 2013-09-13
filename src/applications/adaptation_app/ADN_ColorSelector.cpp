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
#include "ADN_Connector_String.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector constructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_ColorSelector::ADN_ColorSelector( QWidget* parent, bool fixedSize )
    : QPushButton( parent )
{
    if( fixedSize )
    {
        setMinimumSize( 25, 25 );
        setMaximumWidth( 3 * height() );
    }
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
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
// Name: ADN_ColorSelector::setText
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::setText( const QString& string )
{
    std::string color( string.toStdString() );
    boost::replace_all( color, "0x", "#" );
    color_.setNamedColor( color.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::paintEvent
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::paintEvent( QPaintEvent *e )
{
    QPushButton::paintEvent( e );
    QPainter painter( this );
    QRect rc( 6, 6, painter.window().width() - 12, painter.window().height() - 12 );
    painter.setPen( Qt::darkBlue );
    painter.fillRect( rc, QBrush( QColor( color_ ) ) );
    painter.drawRect( rc );
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::OnClick
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
void ADN_ColorSelector::OnClick()
{
    QColor color = QColorDialog::getColor( color_, this, tr( "Select color" ) );
    if( color.isValid() )
    {
        color_ = color;
        repaint();
        static_cast< ADN_Connector_String< ADN_ColorSelector >* >( pConnector_ )->SetDataChanged( "0x" + color_.name().right( 6 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ColorSelector::Warn
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_ColorSelector::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
