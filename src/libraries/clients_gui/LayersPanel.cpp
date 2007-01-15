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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayersPanel constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::LayersPanel( QWidget* parent )
    : PreferencePanel_ABC( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayersPanel destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
LayersPanel::~LayersPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::AddLayer
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::AddLayer( const QString& name, Layer2d_ABC& layer )
{
    QGroupBox* alphaBox = new QGroupBox( 1, Qt::Vertical, name, this );
    QSlider* slider = new QSlider( 0, 100, 1, 100, Qt::Horizontal, alphaBox );
    connect( slider, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
    layers_.push_back( T_Layer( &layer, slider ) );
    current_.push_back( 1 );
    new_    .push_back( 1 );
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::Commit
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::Commit()
{
    for( unsigned i = 0; i < layers_.size(); ++i )
        current_[ i ] = new_[ i ];
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
    }
}

// -----------------------------------------------------------------------------
// Name: LayersPanel::OnValueChanged
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void LayersPanel::OnValueChanged( int )
{
    for( unsigned i = 0; i < layers_.size(); ++i )
    {
        new_[ i ] = layers_[ i ].second->value() * 0.01f;
        layers_[ i ].first->SetAlpha( new_[ i ] );
    }
}
