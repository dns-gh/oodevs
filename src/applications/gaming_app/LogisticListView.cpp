// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticListView.h"

#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/UnitMagicAction.h"

#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"

#include "gaming/LogisticLinks.h"
#include "gaming/StaticModel.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: LogisticListView constructor
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
LogisticListView::LogisticListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory,
                                    const Profile_ABC& profile, const gui::EntitySymbols& symbols, actions::ActionsModel& actionsModel,
                                    const ::StaticModel& staticModel, const Time_ABC& simulation )
    : gui::LogisticListView( pParent, controllers, factory, profile, symbols )
    , actionsModel_( actionsModel )
    , static_      ( staticModel )
    , simulation_  ( simulation)
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView destructor
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
LogisticListView::~LogisticListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyUpdated
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::NotifyUpdated( const LogisticLinks& links )
{
    const Entity_ABC& entity = links.GetEntity();
    FindOrCreateOrReplace( &entity );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::RetrieveSuperior
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
const Entity_ABC* LogisticListView::RetrieveSuperior( const Entity_ABC& entity ) const
{
    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();
    return ( links ) ? links->GetCurrentSuperior() : 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::SetSuperior
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::SetSuperior( const Entity_ABC& entity, const Entity_ABC* superior )
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
    UnitMagicAction* action = new UnitMagicAction( entity, actionType, controllers_.controller_, tr( "Change Logistic Links"), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();

    parameters::Identifier* nominalParam = new parameters::Identifier( it.NextElement(), ( superior ) ? ( links && links->GetNominalSuperior() ) ? links->GetNominalSuperior()->GetId() : superior->GetId() : 0 );
    parameters::Identifier* currentParam = new parameters::Identifier( it.NextElement(), ( superior ) ? superior->GetId() : 0 );
    nominalParam->Set( superior != 0 );
    currentParam->Set( superior != 0 );
    action->AddParameter( *nominalParam );
    action->AddParameter( *currentParam );

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &entity, false ) );
    action->RegisterAndPublish( actionsModel_ );
}
