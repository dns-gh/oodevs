// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TacticalListView.h"
#include "ModelBuilder.h"
#include "preparation/Team.h"
#include "preparation/AutomatDecisions.h"
#include "preparation/FormationLevels.h"
#include "preparation/TacticalHierarchies.h"
#include "preparation/Level.h"
#include "preparation/EntityCommunications.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/Tools.h"

using namespace gui;
#include "moc_TacticalListView.cpp"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder, const FormationLevels& levels )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory )
    , factory_( factory )
    , modelBuilder_( modelBuilder )
    , levels_( levels )
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
void TacticalListView::Display( const Entity_ABC& agent, gui::ValuedListItem* item )
{
    item->setRenameEnabled( 0, true );
    const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >();
    if( decisions )
        item->setPixmap( 0, decisions->IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
    gui::HierarchyListView< kernel::TacticalHierarchies >::Display( agent, item );
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
    menu->insertItem( tools::translate( "Preparation", "Cr�er un camp" ), &modelBuilder_, SLOT( OnCreate() ) );
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
    QPopupMenu* subMenu = menu.SubMenu( "Commande", tools::translate( "Preparation", "Cr�er une formation" ) );
    const HierarchyLevel_ABC* level = levels_.GetRoot();
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
    const HierarchyLevel_ABC* level = &formation.GetLevel();
    if( level && level->GetNext() )
    {
        QPopupMenu* subMenu = menu.SubMenu( "Commande", tools::translate( "Preparation", "Cr�er une formation" ) );
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
// Name: TacticalListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
    const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &item );
    if( agent )
        return Drop( *agent, target );

    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &item );
    if( automat )
        return Drop( *automat, target );

    const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &item );
    if( formation )
        return Drop( *formation, target );
    return false;
}

namespace 
{
    Entity_ABC& GetFirstCommunicationChild( const Entity_ABC& entity )
    {
        Iterator< const Entity_ABC& > it = entity.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
        if( it.HasMoreElements() )
            return const_cast< Entity_ABC& >( it.NextElement() );
        throw std::runtime_error( "No communication child found" );
    }

    void UpdateCommunicationHierarchies( Entity_ABC& entity, const Entity_ABC& superior )
    {
        const Entity_ABC& tacticalTop = superior.Get< kernel::TacticalHierarchies >().GetTop();
        kernel::CommunicationHierarchies* com = entity.Retrieve< kernel::CommunicationHierarchies >();

        if( com && &com->GetTop() != &tacticalTop )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( GetFirstCommunicationChild( tacticalTop ) );

        Iterator< const Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            UpdateCommunicationHierarchies( const_cast< Entity_ABC& >( it.NextElement() ), superior );
    }

    bool ChangeSuperior( const Entity_ABC& entity, const Entity_ABC& superior ) // $$$$ SBO 2006-09-28: cast-machine
    {
        Entity_ABC& ent = const_cast< Entity_ABC& >( entity );
        if( kernel::TacticalHierarchies* hierarchies = ent.Retrieve< kernel::TacticalHierarchies >() )
        {
            UpdateCommunicationHierarchies( ent, superior );
            static_cast< ::TacticalHierarchies* >( hierarchies )->ChangeSuperior( const_cast< Entity_ABC& >( superior ) );
            return true;
         }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Agent_ABC& item, const Entity_ABC& target )
{
    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target );
    if( automat )
        return ChangeSuperior( item, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target )
{
    const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &target );
    if( formation )
        return ChangeSuperior( item, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Formation_ABC& formation, const Entity_ABC& target )
{
    const Formation_ABC* targetFormation = dynamic_cast< const Formation_ABC* >( &target );
    if( targetFormation )
    {
        if( formation.GetLevel() < targetFormation->GetLevel() )
            return ChangeSuperior( formation, target );
        return false;
    }

    const Team_ABC* team = dynamic_cast< const Team_ABC* >( &target );
    if( team )
        return ChangeSuperior( formation, target );
    return false;

}
