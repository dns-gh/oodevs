// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TacticalListView.h"
#include "moc_TacticalListView.cpp"
#include "ModelBuilder.h"
#include "preparation/Team.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AutomatDecisions.h"
#include "preparation/FormationLevels.h"
#include "preparation/Level.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Hierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder, const FormationLevels& levels )
    : gui::AgentListView( pParent, controllers, factory )
    , factory_( factory )
    , modelBuilder_( modelBuilder )
    , levels_( levels )
    , selectedFormation_( controllers )
{
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Hierarchies& hierarchy, gui::ValuedListItem* item )
{
    if( ! hierarchy.GetSuperior() )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Entity_ABC& agent, gui::ValuedListItem* item )
{
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 0, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    gui::AgentListView::Display( agent, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const ModelLoaded& )
{
    clear(); // $$$$ SBO 2006-09-28: ajouter ModelUnLoaded + disconnect ?
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const Entity_ABC* agent = & decisions.GetAgent();
    gui::ValuedListItem* item = gui::FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 0, decisions.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() );
    if( item )
        item->SetNamed( entity );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::keyPressEvent
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void TacticalListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        if( modelBuilder_.OnDelete() )
            return;
    QListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnContextMenuRequested
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TacticalListView::OnContextMenuRequested( QListViewItem* item, const QPoint& pos, int index )
{
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Preparation", "Créer un camp" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Team_ABC&, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    QPopupMenu* subMenu = menu.SubMenu( "Commande", tools::translate( "Preparation", "Créer une formation" ) );
    const Level* level = levels_.GetRoot();
    while( level && ( level = level->GetNext() ) )
        subMenu->insertItem( level->GetName(), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Supprimer" ), &modelBuilder_, SLOT( OnDelete() ) );
}
    
// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    if( &formation != selectedFormation_ )
        selectedFormation_ = &formation;
    const Level* level = levels_.Resolve( selectedFormation_->GetLevel() );
    if( level && level->GetNext() )
    {
        QPopupMenu* subMenu = menu.SubMenu( "Commande", tools::translate( "Preparation", "Créer une formation" ) );
        while( level && ( level = level->GetNext() ) )
            subMenu->insertItem( level->GetName(), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    }
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Supprimer" ), &modelBuilder_, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( agent.Retrieve< AutomatDecisions >() )
    {
        if( ! agent.Retrieve< AutomatDecisions >()->IsEmbraye() )
            menu.InsertItem( "Commande", tr( "Embrayer" ), this, SLOT( Engage() ) );
        else
            menu.InsertItem( "Commande", tr( "Debrayer" ), this, SLOT( Disengage() ) );
    }
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Supprimer" ), &modelBuilder_, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Engage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Engage()
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( valuedItem && valuedItem->IsA< const Agent_ABC* >() )
    {
        Agent_ABC& agent = const_cast< Agent_ABC& >( *valuedItem->GetValue< const Agent_ABC* >() );
        agent.Get< AutomatDecisions >().Engage();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Disengage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Disengage()
{
    gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( valuedItem && valuedItem->IsA< const Agent_ABC* >() )
    {
        Agent_ABC& agent = const_cast< Agent_ABC& >( *valuedItem->GetValue< const Agent_ABC* >() );
        agent.Get< AutomatDecisions >().Disengage();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::BeforeSelection
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::BeforeSelection()
{
    selectedFormation_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalListView::AfterSelection
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::AfterSelection()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TacticalListView::Select
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::Select( const Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
//        return Drop(*agent, *group );
    return false;
}
