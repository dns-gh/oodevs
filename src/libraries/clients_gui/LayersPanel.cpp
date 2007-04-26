// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LayersPanel.h"
#include "moc_LayersPanel.cpp"
#include "Layer_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayersPanel constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::LayersPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent )
    , controllers_( controllers )
    , options_( controllers.options_ )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::~LayersPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::AddLayer( const QString& name, Layer_ABC& layer )
{
    QHBox* alphaBox = new QHBox( this );
    QLabel* label = new QLabel( name, alphaBox );
    label->setMinimumWidth( 100 );
    label->setMaximumWidth( 100 );
    QSlider* slider = new QSlider( 0, 100, 1, 100, Qt::Horizontal, alphaBox );
    connect( slider, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged() ) );
    layers_.push_back( T_Layer( &layer, slider ) );
    current_.push_back( 1 );
    new_    .push_back( 1 );
    names_  .push_back( "Layers/" + std::string( name.ascii() ) + "/Alpha" );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Commit
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Commit()
{
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        current_[ i ] = new_[ i ];
        options_.Change( names_[ i ], current_[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Reset
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Reset()
{
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        new_[ i ] = current_[ i ];
        layers_[ i ].first->SetAlpha( new_[ i ] );
        layers_[ i ].second->setValue( new_[ i ] * 100 );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnValueChanged
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::OnValueChanged()
{
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        new_[ i ] = layers_[ i ].second->value() * 0.01f;
        layers_[ i ].first->SetAlpha( new_[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OptionChanged
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void LayersPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    const QString option( name.c_str() );
    if( !option.startsWith( "Layers/" ) )
        return;
    for( unsigned i = 0; i < layers_.size(); ++i )
        if( names_[i] == name )
        {
            const float alpha = value.To< float >();
            new_[ i ] = current_[ i ] = alpha;
            layers_[ i ].second->setValue( alpha * 100 );
            layers_[ i ].first->SetAlpha( new_[ i ] );
            return;
        }
}
