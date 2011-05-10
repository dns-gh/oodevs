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
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "preparation/Team.h"
#include "preparation/AutomatDecisions.h"
#include "preparation/TacticalHierarchies.h"
#include "preparation/EntityCommunications.h"
#include "preparation/CommandPostAttributes.h"
#include "clients_kernel/Level.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "icons.h"

using namespace gui;
#include "moc_TacticalListView.cpp"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder, const FormationLevels& levels )
    : gui::HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , factory_( factory )
    , modelBuilder_( modelBuilder )
    , levels_( levels )
    , lock_( MAKE_PIXMAP( lock ) )
    , commandPost_( MAKE_PIXMAP( commandpost ) )
{
    controllers_.Register( *this );
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
    connect( this, SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( QListViewItem*, int, const QString& ) ) );

    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Entity_ABC& entity, gui::ValuedListItem* item )
{
    item->setRenameEnabled( 0, true );
    if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( entity.Retrieve< CommandPostAttributes >() )
        item->setPixmap( 1, commandPost_ );
    HierarchyListView< kernel::TacticalHierarchies >::Display( entity, item );
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
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() ) )
    {
        item->SetNamed( entity );
        if( entity.Retrieve< CommandPostAttributes >() )
            item->setPixmap( 1, commandPost_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::keyPressEvent
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void TacticalListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        modelBuilder_.DeleteEntity( *((ValuedListItem*)selectedItem())->GetValue< const Entity_ABC >() );
    else
        QListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnContextMenuRequested
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TacticalListView::OnContextMenuRequested( QListViewItem* item, const QPoint& pos, int index )
{
    HierarchyListView_ABC::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Entity_ABC&, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Team_ABC&, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    QPopupMenu* subMenu = menu.SubMenu( "Creation", tr( "Create formation" ) );
    const HierarchyLevel_ABC* level = levels_.GetRoot();
    while( level && ( level = level->GetNext() ) )
        subMenu->insertItem( tools::translate( "models::app6", level->GetName().ascii() ), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
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
        QPopupMenu* subMenu = menu.SubMenu( "Creation", tr( "Create formation" ) );
        while( level && ( level = level->GetNext() ) )
            subMenu->insertItem( tools::translate( "models::app6", level->GetName().ascii() ), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnRename
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void TacticalListView::OnRename()
{
    if( selectedItem() )
        selectedItem()->startRename( 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
    {
        if( ! decisions->IsEmbraye() )
            menu.InsertItem( "Command", tr( "Engage" ), this, SLOT( Engage() ) );
        else if( decisions->CanBeOrdered() )
            menu.InsertItem( "Command", tr( "Disengage" ), this, SLOT( Disengage() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Engage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Engage()
{
    if( gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() ) )
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            decisions->Engage();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Disengage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Disengage()
{
    if( gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() ) )
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            decisions->Disengage();
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
        tools::Iterator< const Entity_ABC& > it = entity.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
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

        tools::Iterator< const Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
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
    {
        bool superiorChange = ChangeSuperior( item, target );
        kernel::CommunicationHierarchies* com = const_cast< Agent_ABC& >( item ).Retrieve< kernel::CommunicationHierarchies >();
        if( com )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( const_cast< Entity_ABC& >( target ) );
        return superiorChange;
    }
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
