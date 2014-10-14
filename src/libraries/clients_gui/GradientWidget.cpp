// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientWidget.h"
#include "moc_GradientWidget.cpp"
#include "ColorButton.h"
#include "GradientButton.h"
#include "SignalAdapter.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientWidget constructor
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
GradientWidget::GradientWidget( const QString& gradientEditorName,
                                bool disableState,
                                const GradientItem::T_Drawer& itemDrawer,
                                QWidget* parent /* = 0 */ )
    : QWidget( parent )
{
    gradientButton_ = new GradientButton( gradientEditorName, disableState, itemDrawer );
    auto color = new ColorButton( gradientEditorName + "-color" );
    color->setMaximumHeight( 30 );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->addWidget( gradientButton_ );
    layout->addWidget( color );
    layout->setAlignment( Qt::AlignCenter );
    setMaximumHeight( 100 );

    connect( gradientButton_, SIGNAL( GradientChanged() ), this, SIGNAL( GradientChanged() ) );
    connect( this, SIGNAL( FitToViewPortChanged( int ) ), gradientButton_, SLOT( OnFitToViewPortChanged( int ) ) );
    gui::connect( gradientButton_, SIGNAL( SelectionChanged( const QColor& ) ), [=](){
        color->SetColor( gradientButton_->GetSelectedColor() );
    } );
    gui::connect( color, SIGNAL( ColorChanged( const QColor& ) ), [=](){
        gradientButton_->SetSelectedColor( color->GetColor() );
    } );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget destructor
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
GradientWidget::~GradientWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::LoadGradient
// Created: ABR 2014-10-06
// -----------------------------------------------------------------------------
void GradientWidget::LoadGradient( const std::shared_ptr< Gradient >& gradient )
{
    gradientButton_->LoadGradient( gradient );
}
