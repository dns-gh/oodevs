// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObstaclePrototype_ABC.h"
#include "moc_ObstaclePrototype_ABC.cpp"
#include "ValuedListItem.h"
#include "Tools.h"

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
    : ObjectAttributePrototype_ABC( parent, tools::translate( "ObstaclePrototype_ABC", "Obstacle" ) )
{
    {
        new QLabel( tools::translate( "ObstaclePrototype_ABC", "Obstacle type:" ), this );
        types_ = new ValuedComboBox< E_DemolitionTargetType >( this );
    }
    {
        QLabel* label = new QLabel( tools::translate( "ObstaclePrototype_ABC", "Reserved obstacle activated:" ), this );
        activation_ = new QCheckBox( this );
        label->hide();
        activation_->hide();
        connect( this, SIGNAL( ToggleActivable( bool ) ), label, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleActivable( bool ) ), activation_, SLOT( setShown( bool ) ) );
    }
    {
        QLabel* label = new QLabel( tools::translate( "ObstaclePrototype_ABC", "Activation time:" ), this );
        activationTime_ = new QTimeEdit( this );
        label->hide();
        activationTime_->hide();
        connect( this, SIGNAL( ToggleActivable( bool ) ), label, SLOT( setShown( bool ) ) );
        connect( this, SIGNAL( ToggleActivable( bool ) ), activationTime_, SLOT( setShown( bool ) ) );
        connect( activation_, SIGNAL( toggled( bool ) ), label, SLOT( setDisabled( bool ) ) );
        connect( activation_, SIGNAL( toggled( bool ) ), activationTime_, SLOT( setDisabled( bool ) ) );
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
    QGroupBox::showEvent( e );
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

    return 3600 * activationTime_->time().hour() +
           60 * activationTime_->time().minute() +
           activationTime_->time().second();
}