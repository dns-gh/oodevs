// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticTreeView.h"
#include "actions/ActionsModel.h"
#include "actions/UnitMagicAction.h"
#include "actions/Identifier.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/LogisticLinks.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: LogisticTreeView constructor
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
LogisticTreeView::LogisticTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver,
                                    const gui::EntitySymbols& symbols, const StaticModel& staticModel,
                                    const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent /*= 0*/ )
    : gui::LogisticTreeView( objectName, controllers, profile, modelObserver, symbols, parent )
    , actionsModel_( actionsModel )
    , static_      ( staticModel )
    , simulation_  ( simulation )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView destructor
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
LogisticTreeView::~LogisticTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::NotifyUpdated
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
void LogisticTreeView::NotifyUpdated( const LogisticLinks& links )
{
    CreateOrReplace( links.GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::RetrieveSuperior
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogisticTreeView::RetrieveSuperior( const kernel::Entity_ABC& entity ) const
{
    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();
    return ( links ) ? links->GetCurrentSuperior() : 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::SetSuperior
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
void LogisticTreeView::SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior )
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();

    actions::parameters::Identifier* nominalParam = new actions::parameters::Identifier( it.NextElement(), ( superior ) ? ( links && links->GetNominalSuperior() ) ? links->GetNominalSuperior()->GetId() : superior->GetId() : 0 );
    actions::parameters::Identifier* currentParam = new actions::parameters::Identifier( it.NextElement(), ( superior ) ? superior->GetId() : 0 );
    nominalParam->Set( superior != 0 );
    currentParam->Set( superior != 0 );
    action->AddParameter( *nominalParam );
    action->AddParameter( *currentParam );

    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, &entity, false ) );
    actionsModel_.Publish( *action );
}
