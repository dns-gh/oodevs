// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "InhabitantAffinities.h"

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
// Name: InhabitantAffinities constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
InhabitantAffinities::InhabitantAffinities( TeamsModel& teams )
    : teams_ ( teams )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantAffinities destructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
InhabitantAffinities::~InhabitantAffinities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantAffinities::DoUpdate
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void InhabitantAffinities::DoUpdate( const sword::PopulationUpdate& message )
{
    for( int i = 0; i < message.adhesions_size(); ++i )
    {
        const sword::PartyAdhesion& adhesion = message.adhesions( i );
        affinities_[ adhesion.party().id() ] = adhesion.value();
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantAffinities::Display
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void InhabitantAffinities::Display(kernel::Displayer_ABC* displayer) const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        kernel::Team_ABC& team = teams_.GetTeam( it->first );
        displayer->Display( tools::translate( "Inhabitant", "Affinity %1:" ).arg( team.GetName() ), it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantAffinities::FillDialog
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void InhabitantAffinities::CreateAffinitiesSpinBoxs( QGrid* grid, T_AffinitiesSpinBoxs& spinboxs )
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
// Name: InhabitantAffinities::FillParameterList
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void InhabitantAffinities::FillParameterList( actions::parameters::ParameterList* parameterList ) const
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        actions::parameters::ParameterList& list = parameterList->AddList( "Affinity" );
        list.AddIdentifier( "ID", it->first );
        list.AddNumeric( "Value", it->second );
    }
}
