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
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools.h"
#include <qgrid.h>
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
Affinities::Affinities( kernel::Controller& controller, TeamsModel& teams )
    : teams_     ( teams )
    , controller_( controller )
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
    for( int i = 0; i < message.adhesions_size(); ++i )
    {
        const sword::PartyAdhesion& adhesion = message.adhesions( i );
        affinities_[ adhesion.party().id() ] = adhesion.value();
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::CrowdUpdate& message )
{
    for( int i = 0; i < message.adhesions_size(); ++i )
    {
        const sword::PartyAdhesion& adhesion = message.adhesions( i );
        affinities_[ adhesion.party().id() ] = adhesion.value();
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::DoUpdate
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
void Affinities::DoUpdate( const sword::UnitAttributes& message )
{
    for( int i = 0; i < message.adhesions_size(); ++i )
    {
        const sword::PartyAdhesion& adhesion = message.adhesions( i );
        affinities_[ adhesion.party().id() ] = adhesion.value();
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Affinities::FillDialog
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void Affinities::CreateAffinitiesSpinBoxs( QGrid* grid, std::map< unsigned long, gui::DecimalSpinBoxAndSlider* >& spinboxs )
{
    for( IT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
        new QLabel( team.GetName(), grid );
        spinboxs[ it->first ] = new gui::DecimalSpinBoxAndSlider( grid, it->second, 2, -1.f, 1.f, 0.01f, Qt::Horizontal, Qt::Horizontal, true );
        spinboxs[ it->first ]->setMargin( 5 );
        spinboxs[ it->first ]->setSpacing( 5 );
    }
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
// Name: Affinities::DisplayInSummary
// Created: LGY 2011-03-15
// -----------------------------------------------------------------------------
void Affinities::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
       displayer.Display( tools::translate( "Affinities", "Affinity %1:" ).arg( team.GetName() ), it->second );
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
