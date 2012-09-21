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

//// -----------------------------------------------------------------------------
//// Name: LogisticTreeView constructor
//// Created: ABR 2012-09-21
//// -----------------------------------------------------------------------------
//LogisticTreeView::LogisticTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver,
//                                    const gui::EntitySymbols& symbols, actions::ActionsModel& actionsModel, const StaticModel& staticModel,
//                                    const kernel::Time_ABC& simulation, QWidget* parent /* = 0 */ )
//    : gui::LogisticTreeView( controllers, profile, modelObserver, symbols, parent )
//    , actionsModel_( actionsModel )
//    , staticModel_ ( staticModel )
//    , simulation_  ( simulation )
//{
//    // NOTHING
//}
//
//// -----------------------------------------------------------------------------
//// Name: LogisticTreeView destructor
//// Created: ABR 2012-09-21
//// -----------------------------------------------------------------------------
//LogisticTreeView::~LogisticTreeView()
//{
//    // NOTHING
//}
//
//// -----------------------------------------------------------------------------
//// Name: LogisticTreeView::NotifyUpdated
//// Created: ABR 2012-09-21
//// -----------------------------------------------------------------------------
//void LogisticTreeView::NotifyUpdated( const LogisticLinks& links )
//{
//    CreateOrReplace( links.GetEntity() );
//}
//
//// -----------------------------------------------------------------------------
//// Name: LogisticTreeView::RetrieveSuperior
//// Created: ABR 2012-09-21
//// -----------------------------------------------------------------------------
//const kernel::Entity_ABC* LogisticTreeView::RetrieveSuperior( const kernel::Entity_ABC& entity ) const
//{
//    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();
//    return ( links ) ? links->GetCurrentSuperior() : 0;
//}
//
//// -----------------------------------------------------------------------------
//// Name: LogisticTreeView::SetSuperior
//// Created: ABR 2012-09-21
//// -----------------------------------------------------------------------------
//void LogisticTreeView::SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior )
//{
//    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
//    UnitMagicAction* action = new UnitMagicAction( entity, actionType, controllers_.controller_, tr( "Change Logistic Links"), true );
//    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
//    const LogisticLinks* links = entity.Retrieve< LogisticLinks >();
//
//    parameters::Identifier* nominalParam = new parameters::Identifier( it.NextElement(), ( superior ) ? ( links && links->GetNominalSuperior() ) ? links->GetNominalSuperior()->GetId() : superior->GetId() : 0 );
//    parameters::Identifier* currentParam = new parameters::Identifier( it.NextElement(), ( superior ) ? superior->GetId() : 0 );
//    nominalParam->Set( superior != 0 );
//    currentParam->Set( superior != 0 );
//    action->AddParameter( *nominalParam );
//    action->AddParameter( *currentParam );
//
//    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
//    action->Attach( *new ActionTasker( &entity, false ) );
//    action->RegisterAndPublish( actionsModel_ );
//}
