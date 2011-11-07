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
#include "PreparationProfile.h"
#include "preparation/AutomatDecisions.h"
#include "preparation/TacticalHierarchies.h"
#include "preparation/EntityCommunications.h"
#include "preparation/CommandPostAttributes.h"
#include "clients_kernel/Level.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "icons.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, ModelBuilder& modelBuilder, const FormationLevels& levels )
    : HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , factory_     ( factory )
    , modelBuilder_( modelBuilder )
    , levels_      ( levels )
    , lock_        ( MAKE_PIXMAP( lock ) )
    , commandPost_ ( MAKE_PIXMAP( commandpost ) )
{
    controllers_.Register( *this );
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
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
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const CommandPostAttributes* pAttributes = entity.Retrieve< CommandPostAttributes >() )
            return pAttributes->IsCommandPost();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    item->setRenameEnabled( 0, true );
    if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
        item->setPixmap( 1, decisions->IsEmbraye() ? lock_ : QPixmap() );
    else if( IsCommandPost( entity ) )
        item->setPixmap( 1, commandPost_ );
    HierarchyListView< kernel::TacticalHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const Entity_ABC* agent = & decisions.GetAgent();
    ValuedListItem* item = FindItem( agent, firstChild() );
    if( item )
        item->setPixmap( 1, decisions.IsEmbraye() ? lock_ : QPixmap() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const Entity_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
    {
        item->SetNamed( entity );
        if( IsCommandPost( entity ) )
            item->setPixmap( 1, commandPost_ );
        else
            item->setPixmap( 1, QPixmap() );
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
        Q3ListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnContextMenuRequested
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TacticalListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    HierarchyListView_ABC::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    Q3PopupMenu* menu = new Q3PopupMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Entity_ABC&, ContextMenu& menu )
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
    if( const HierarchyLevel_ABC* root = levels_.GetRoot() )
        AddFormationMenu( menu, *root );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    AddFormationMenu( menu, formation.GetLevel() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::AddFormationMenu
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void TacticalListView::AddFormationMenu( ContextMenu& menu, const HierarchyLevel_ABC& root )
{
    if( !isVisible() )
        return;
    Q3PopupMenu* subMenu = menu.SubMenu( "Creation", tr( "Create formation" ) );
    for( const HierarchyLevel_ABC* level = &root; level; level = level->GetNext() )
        subMenu->insertItem( tools::findTranslation( "models::app6", level->GetName().ascii() ), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
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
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) )
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
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) )
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

    const Ghost_ABC* ghost = dynamic_cast< const Ghost_ABC* >( &item );
    if( ghost )
        return Drop( *ghost, target );

    return false;
}

namespace
{
    Entity_ABC& GetFirstCommunicationChild( const Entity_ABC& entity )
    {
        tools::Iterator< const Entity_ABC& > it = entity.Get< CommunicationHierarchies >().CreateSubordinateIterator();
        if( it.HasMoreElements() )
            return const_cast< Entity_ABC& >( it.NextElement() );
        throw std::runtime_error( "No communication child found" );
    }

    void UpdateCommunicationHierarchies( Entity_ABC& entity, const Entity_ABC& superior )
    {
        const Entity_ABC& tacticalTop = superior.Get< kernel::TacticalHierarchies >().GetTop();
        CommunicationHierarchies* com = entity.Retrieve< CommunicationHierarchies >();

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
        CommunicationHierarchies* com = const_cast< Agent_ABC& >( item ).Retrieve< CommunicationHierarchies >();
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
bool TacticalListView::Drop( const Automat_ABC& item, const Entity_ABC& target )
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
        return ChangeSuperior( formation, target );

    const Team_ABC* team = dynamic_cast< const Team_ABC* >( &target );
    if( team )
        return ChangeSuperior( formation, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target )
{
    const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &target );
    if( formation && item.GetGhostType() == eGhostType_Automat )
        return ChangeSuperior( item, target );

    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target );
    if( automat && item.GetGhostType() == eGhostType_Agent )
        return ChangeSuperior( item, target );

    return false;
}
