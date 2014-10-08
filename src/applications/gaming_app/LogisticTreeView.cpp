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
#include "moc_LogisticTreeView.cpp"
#include "actions/ActionsModel.h"
#include "actions/UnitMagicAction.h"
#include "actions/Identifier.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "clients_gui/ChangeSuperiorDialog.h"
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
LogisticTreeView::LogisticTreeView( const QString& objectName,
                                    kernel::Controllers& controllers,
                                    const kernel::Profile_ABC& profile,
                                    gui::ModelObserver_ABC& modelObserver,
                                    const gui::EntitySymbols& symbols,
                                    const StaticModel& staticModel,
                                    const kernel::Time_ABC& simulation,
                                    actions::ActionsModel& actionsModel,
                                    gui::ChangeSuperiorDialog& changeSuperiorDialog,
                                    QWidget* parent /*= 0*/ )
    : gui::LogisticTreeView( objectName, controllers, profile, modelObserver, symbols, changeSuperiorDialog, parent )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
{
    changeSuperiorDialog.SetLogisticCurrentExtractor(
        [&] ( const kernel::Entity_ABC& entity ) {
            return RetrieveSuperior( entity );
        } );
    changeSuperiorDialog.SetLogisticNominalFunctors(
        [] ( const kernel::Entity_ABC& entity ) -> const kernel::Entity_ABC*  {
            auto links = entity.Retrieve< LogisticLinks >();
            return links ? links->GetNominalSuperior() : 0;
        },
        [&] ( kernel::Entity_ABC& entity, const kernel::Entity_ABC* nominalSuperior, const kernel::Entity_ABC* currentSuperior ) {
            SetSuperior( entity, nominalSuperior, currentSuperior );
        } );
    controllers_.Update( *this );  
    actionsModel_.RegisterHandler( [&]( const sword::SimToClient& message )
    {
        if( contexts_.empty() || 
            !message.message().has_unit_magic_action_ack() || !PopContext( message.context() ) )
            return;
        auto ack = message.message().unit_magic_action_ack();
        auto errorCode = ack.error_code();
        if( errorCode != sword::UnitActionAck_ErrorCode_no_error )
            QMessageBox::warning( this, tr( "SWORD" ), GetErrorText( ack ) );
    } );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::GetErrorText
// Created: LDC 2014-10-07
// -----------------------------------------------------------------------------
QString LogisticTreeView::GetErrorText( const sword::UnitMagicActionAck& ack )
{
    switch( ack.error_code() )
    {
        case sword::UnitActionAck::error_invalid_parameter :
            return tr( "Invalid subordinate automat" );
        case sword::UnitActionAck::error_not_log_automat:
            return tr( "Target is not a logistic entity" );
        case sword::UnitActionAck::error_hierarchy_cycle:
            return tr( "Action cancelled: It would create a cycle in the logistic hierarchy" );
        default:
            return QString( ack.has_error_msg() ? QString::fromStdString( ack.error_msg() ) : tr( "The operation couldn't be performed" ) );
    }
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
    auto links = entity.Retrieve< LogisticLinks >();
    return links ? links->GetCurrentSuperior() : 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::SetSuperior
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
void LogisticTreeView::SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* currentsuperior, const kernel::Entity_ABC* nominalSuperior )
{
    const kernel::Entity_ABC* superior = nominalSuperior;
    if( !superior )
    {
        const LogisticLinks* links = entity.Retrieve< LogisticLinks >();
        superior = links && links->GetNominalSuperior() ? links->GetNominalSuperior() : currentsuperior;
    }
    contexts_.push_back( actionsModel_.PublishChangeLogisticLinks( entity, superior, currentsuperior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticTreeView::PopContext
// Created: LDC 2014-10-07
// -----------------------------------------------------------------------------
bool LogisticTreeView::PopContext( int context )
{
    auto it = std::find( contexts_.begin(), contexts_.end(), context );
    if( it == contexts_.end() )
        return false;
    contexts_.erase( it );
    return true;
}
