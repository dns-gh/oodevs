// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalTreeView.h"
#include "moc_TacticalTreeView.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Options.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel, const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent /*= 0*/ )
    : gui::TacticalTreeView( controllers, profile, modelObserver, symbols, parent )
    , static_( staticModel )
    , simulation_( simulation )
    , actionsModel_( actionsModel )
    , changeSuperiorDialog_( 0 )
{
    controllers_.Update( *this );
    setEditTriggers( 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( hierarchies->GetSuperior() != 0 && dynamic_cast< const kernel::Object_ABC* >( &entity ) == 0 )
            menu.InsertItem( "Command", tr( "Change superior" ), this, SLOT( OnChangeSuperior() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyUpdated
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyUpdated( const kernel::AutomatDecisions_ABC& /*decisions*/ )
{
    proxyModel_->invalidate();
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::drawRow
// Created: JSR 2012-10-10
// -----------------------------------------------------------------------------
void TacticalTreeView::drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const
{
    QStandardItem* item = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( index ) );
    if( item && item->data( gui::StandardModel::MimeTypeRole).isValid() &&
        item->data( gui::StandardModel::MimeTypeRole).toString() == typeid( kernel::Agent_ABC ).name() )
    {
        if( const kernel::Agent_ABC* agent = dataModel_.GetDataFromItem< const kernel::Agent_ABC >( *item ) )
        {
            if( const Attributes* attributes = static_cast< const Attributes* >( agent->Retrieve< kernel::Attributes_ABC >() ) )
            {
                if( attributes->nOpState_ == eOperationalStatus_DetruitTotalement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() ) );
                else if( attributes->nOpState_ == eOperationalStatus_DetruitTactiquement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() ) );
                if( attributes->bNeutralized_ )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() ) );
            }
        }
    }
    gui::TacticalTreeView::drawRow( painter, options, index );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::CanChangeSuperior
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
bool TacticalTreeView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0;
    if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0 || dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::DoChangeSuperior
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        Drop( *agent, superior );
    else if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        Drop( *automat, superior );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnChangeSuperior
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::OnChangeSuperior()
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
        {
            if( !changeSuperiorDialog_ )
                changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, false );
            changeSuperiorDialog_->Show( *entity );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
    {
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() == automat )
            return;
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& >( static_.types_ ).Get( "unit_change_superior" );
        actions::UnitMagicAction* action = new actions::UnitMagicAction( item, actionType, controllers_.controller_, tr( "Unit Change Superior" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Automat( it.NextElement(), *automat, controllers_.controller_ ) );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( &item, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target)
{
    actions::UnitMagicAction* action = 0;
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
    {
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() == automat || &item == automat )
            return;
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& >( static_.types_ ).Get( "change_automat_superior" );
        action = new actions::UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Automat Superior" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Automat( it.NextElement(), *automat, controllers_.controller_ ) );
    }
    else if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
    {
        if( & item.Get< kernel::TacticalHierarchies >().GetUp() == formation )
            return;
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& >( static_.types_ ).Get( "change_formation_superior" );
        action = new actions::UnitMagicAction( item, actionType, controllers_.controller_, tr( "Change Formation Superior" ), true );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Formation( it.NextElement(), *formation, controllers_.controller_ ) );
    }
    if( action )
    {
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( &item, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}
