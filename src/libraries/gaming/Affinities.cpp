// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Affinities.h"
#include "TeamsModel.h"
#include "actions/ParameterList.h"
#include "clients_gui/DecimalSpinBoxAndSlider.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools.h"
#include <qgrid.h>
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::Affinities( kernel::Controller& controller, TeamsModel& teams, kernel::PropertiesDictionary& dico )
    : teams_     ( teams )
    , controller_( controller )
    , dico_      ( dico )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities destructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::~Affinities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::PopulationUpdate& message )
{
    if( message.has_adhesions() )
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }
    CreateDictionary();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::CrowdUpdate& message )
{
    if( message.has_adhesions() )
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }
    CreateDictionary();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_adhesions() )
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }
    CreateDictionary();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::CreateDictionary
// Created: JSR 2011-03-18
// -----------------------------------------------------------------------------
void Affinities::CreateDictionary() const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
        const QString key = tools::translate( "Affinities", "Affinities/%1" ).arg( team.GetName() );
        if( !dico_.HasKey( key ) )
            dico_.Register( *this, key,  it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::FillDialog
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void Affinities::CreateAffinitiesSpinBoxs( QGrid* grid, std::map< unsigned long, gui::DecimalSpinBoxAndSlider* >& spinboxs )
{
    changingAffinities_ = affinities_;
    for( IT_Affinities it = changingAffinities_.begin(); it != changingAffinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
        new QLabel( team.GetName(), grid );
        spinboxs[ it->first ] = new gui::DecimalSpinBoxAndSlider( grid, it->second, 2, -1.f, 1.f, 0.01f, Qt::Horizontal, Qt::Horizontal, true );
        spinboxs[ it->first ]->setMargin( 5 );
        spinboxs[ it->first ]->setSpacing( 5 );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::CancelChanges
// Created: JSR 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::CancelChanges()
{
    changingAffinities_.clear();
}

// -----------------------------------------------------------------------------
// Name: Affinities::ValidateChanges
// Created: JSR 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::ValidateChanges()
{
    affinities_ = changingAffinities_;
    changingAffinities_.clear();
}

// -----------------------------------------------------------------------------
// Name: Affinities::FillParameterList
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void Affinities::FillParameterList( actions::parameters::ParameterList* parameterList ) const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        actions::parameters::ParameterList& list = parameterList->AddList( "Affinity" );
        list.AddIdentifier( "ID", it->first );
        list.AddNumeric( "Value", it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Affinities::HasAffinities
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
bool Affinities::HasAffinities() const
{
    return !affinities_.empty();
}
