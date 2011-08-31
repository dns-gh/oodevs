// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ObstaclePrototype_ABC */

#include "clients_gui_pch.h"
#include "ObstaclePrototype_ABC.h"
#include "moc_ObstaclePrototype_ABC.cpp"
#include "ValuedListItem.h"
#include "Tools.h"
#include "LoadableCheckBox.h"
#include "LoadableTimeEdit.h"

using namespace gui;

namespace
{
    template< typename Enum, typename Combo >
    void Populate( Enum size, Combo& combo )
    {
        combo.Clear();
        for( unsigned int i = 0; i < unsigned int( size ); ++i )
            combo.AddItem( tools::ToString( (Enum)i ), (Enum)i );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ObstaclePrototype_ABC::ObstaclePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::ObstaclePrototype_ABC", "Obstacle" ) )
{
    Q3VBox* vbox = new Q3VBox( this );
    {
        Q3HBox* hBox = new Q3HBox( vbox );
        new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Obstacle type:" ), hBox );
        types_ = new ValuedComboBox< E_DemolitionTargetType >( hBox );
    }
    {
        activation_ = new LoadableCheckBox( tools::translate( "gui::ObstaclePrototype_ABC", "Reserved obstacle activated:" ), vbox );
        activation_->hide();
        connect( this, SIGNAL( ToggleActivable( bool ) ), activation_, SLOT( setShown( bool ) ) );
    }
    {
        Q3HBox* activationBox = new Q3HBox( vbox );
        QLabel* activationLabel = new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ), activationBox );
        activationTime_ = new LoadableTimeEdit( activationBox );
        activationLabel->hide();
        activationTime_->hide();
        Q3HBox* durationBox = new Q3HBox( vbox );
        QLabel* durationLabel = new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Duration:" ), durationBox );
        duration_ = new LoadableTimeEdit( durationBox );
        durationLabel->hide();
        duration_->hide();

        connect( this, SIGNAL( ToggleActivable( bool ) ), activationLabel, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleActivable( bool ) ), activationTime_, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleActivable( bool ) ), durationLabel, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleActivable( bool ) ), duration_, SLOT( setShown( bool ) ) );

        connect( activation_->GetDefaultValueWidget(), SIGNAL( toggled( bool ) ), activationLabel, SLOT( setDisabled( bool ) ) );
        connect( activation_->GetDefaultValueWidget(), SIGNAL( toggled( bool ) ), activationTime_, SLOT( setDisabled( bool ) ) );
        connect( activation_->GetDefaultValueWidget(), SIGNAL( toggled( bool ) ), durationLabel, SLOT( setDisabled( bool ) ) );
        connect( activation_->GetDefaultValueWidget(), SIGNAL( toggled( bool ) ), duration_, SLOT( setDisabled( bool ) ) );
    }
    connect( types_, SIGNAL( activated( int ) ), this, SLOT( OnObstacleTypeChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ObstaclePrototype_ABC::~ObstaclePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::showEvent( QShowEvent* e )
{
    Populate( eNbrDemolitionTargetType, *types_ );
    OnObstacleTypeChanged();
    Q3GroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool ObstaclePrototype_ABC::CheckValidity() const
{
    return types_->count() > 0;
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::OnObstacleTypeChanged
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::OnObstacleTypeChanged()
{
    emit ToggleActivable( types_->GetValue() == eDemolitionTargetType_Reserved );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::OnObstacleTypeChanged
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
bool ObstaclePrototype_ABC::IsActivated() const
{
    if( types_->GetValue() == eDemolitionTargetType_Reserved )
        return activation_->isChecked();
    else if( types_->GetValue() == eDemolitionTargetType_Preliminary )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetActivationTime
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetActivationTime() const
{
    if( IsActivated() )
        return 0;
    QTime time = activationTime_->time();
    return 3600 * time.hour() + 60 * time.minute() + time.second();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetDuration
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetDuration() const
{
    if( IsActivated() )
        return 0;
    QTime time = duration_->time();
    return 3600 * time.hour() + 60 * time.minute() + time.second();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    activation_->SetLoader( loader );
    activationTime_->SetLoader( loader );
}
