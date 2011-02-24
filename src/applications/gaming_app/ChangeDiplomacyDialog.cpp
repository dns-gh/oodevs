// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeDiplomacyDialog.h"
#include "actions/ActionTiming.h"
#include "actions/Enumeration.h"
#include "actions/Identifier.h"
#include "actions/MagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog constructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : gui::DiplomacyDialog_ABC( parent, controllers, profile )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog destructor
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
ChangeDiplomacyDialog::~ChangeDiplomacyDialog()
{
    // NOTHING
}

namespace
{
    sword::EnumDiplomacy ResolveDiplomacy( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return sword::friendly;
        if( karma == kernel::Karma::enemy_ )
            return sword::enemy;
        if( karma == kernel::Karma::neutral_ )
            return sword::neutral;
        return sword::unknown;
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeDiplomacyDialog::SetDiplomacy
// Created: SBO 2008-12-09
// -----------------------------------------------------------------------------
void ChangeDiplomacyDialog::SetDiplomacy( const kernel::Team_ABC& team1, const kernel::Team_ABC& team2, const kernel::Karma& diplomacy ) const
{
    if( team1.Get< kernel::Diplomacies_ABC >().GetDiplomacy( team2 ) == diplomacy )
        return;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_diplomacy" );
    MagicAction* action = new MagicAction( actionType, controllers_.controller_, tools::translate( "ChangeDiplomacyDialog", "Change Diplomacy" ).ascii(), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), team1.GetId() ) );
    action->AddParameter( *new parameters::Identifier( it.NextElement(), team2.GetId() ) );
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), ResolveDiplomacy( diplomacy ) ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->RegisterAndPublish( actionsModel_ );
}
