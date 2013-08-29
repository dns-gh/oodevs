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
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    Q3VBox* vbox = new Q3VBox( this );
    layout->addWidget( vbox );
    {
        Q3HBox* hBox = new Q3HBox( vbox );
        new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Obstacle type:" ), hBox );
        types_ = new ValuedComboBox< E_DemolitionTargetType >( hBox );
    }
    {
        Q3HBox* activationBox = new Q3HBox( vbox );
        QLabel* activationLabel = new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Activation time:" ), activationBox );
        activationTime_ = new LoadableTimeEdit( activationBox );
        activationLabel->show();
        activationTime_->show();
        Q3HBox* activityBox = new Q3HBox( vbox );
        QLabel* activityLabel = new QLabel( tools::translate( "gui::ObstaclePrototype_ABC", "Activity time:" ), activityBox );
        activityTime_ = new LoadableTimeEdit( activityBox );
        activityLabel->show();
        activityTime_->show();

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
    RichGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool ObstaclePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return types_->count() > 0;
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::OnObstacleTypeChanged
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::OnObstacleTypeChanged()
{
    if( activationTime_ && activationTime_->GetDefaultValueWidget() )
        activationTime_->GetDefaultValueWidget()->setTime( QTime() );
    if( activityTime_ && activityTime_->GetDefaultValueWidget() )
        activityTime_->GetDefaultValueWidget()->setTime( QTime() );
    emit ToggleActivable( types_->GetValue() != eDemolitionTargetType_Reserved );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetActivationTime
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetActivationTime() const
{
    QTime time = activationTime_->time();
    return 3600 * time.hour() + 60 * time.minute() + time.second();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetActivityTime
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetActivityTime() const
{
    QTime time = activityTime_->time();
    return 3600 * time.hour() + 60 * time.minute() + time.second();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    activationTime_->SetLoader( loader );
    activityTime_->SetLoader( loader );
}
