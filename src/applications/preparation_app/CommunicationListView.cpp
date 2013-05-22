// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CommunicationListView.h"
#include "moc_CommunicationListView.cpp"
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_gui/ValuedDragObject.h" // LTO
#include "clients_gui/ValuedListItem.h" // LTO
#include "preparation/AutomatCommunications.h"
#include "preparation/KnowledgeGroupCommunications.h" // LTO
#include "clients_kernel/Tools.h"

using namespace kernel;

namespace
{
    int ItemComparator( const gui::ValuedListItem& item1, const gui::ValuedListItem& item2, int /*col*/, bool /*ascending*/ )
    {
        const Entity_ABC* entity1 = item1.GetValue< const Entity_ABC >();
        if( !entity1 )
            return -1;
        const Entity_ABC* entity2 = item2.GetValue< const Entity_ABC >();
        if( !entity2 )
            return 1;
        return entity1->GetId() - entity2->GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::CommunicationListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder )
    : gui::HierarchyListView< kernel::CommunicationHierarchies >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , factory_             ( factory )
    , modelBuilder_        ( modelBuilder )
    , changeSuperiorDialog_( 0 )
{
    controllers_.Register( *this );
    setResizeMode( Q3ListView::AllColumns );
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
    SetComparator( &ItemComparator );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::~CommunicationListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::hideEvent
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void CommunicationListView::hideEvent( QHideEvent* event )
{
    if( changeSuperiorDialog_ )
        changeSuperiorDialog_->hide();
    gui::HierarchyListView< kernel::CommunicationHierarchies >::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() );
    if( item )
    {
        item->SetNamed( entity );
        item->setRenameEnabled( 0, !gui::LongNameHelper::SetItemLongName( entity, *item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyDeleted
// Created: JSR 2012-02-02
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyDeleted( const kernel::KnowledgeGroup_ABC& kg )
{
    const Entity_ABC& top = kg.Get< CommunicationHierarchies >().GetTop();
    const CommunicationHierarchies& teamHierarchy = top.Get< CommunicationHierarchies >();
    tools::Iterator< const Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    bool isFirst = false;
    while( it.HasMoreElements() )
    {
        const Entity_ABC* entity = &it.NextElement();
        if( dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
        {
            if( isFirst )
            {
                gui::ValuedListItem* item = gui::FindItem( entity, firstChild() );
                if( item )
                    item->SetBold( true );
                return;
            }
            if( entity == &kg )
                isFirst = true;
            else
                return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Display
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    if( !item )
        return;
    QColor color = Qt::transparent;
    if( dynamic_cast< const Ghost_ABC* >( &entity ) != 0 )
        color = QColor( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
    item->SetBackgroundColor( color );
    gui::HierarchyListView< kernel::CommunicationHierarchies >::Display( entity, item );
    item->setRenameEnabled( 0, !gui::LongNameHelper::SetItemLongName( entity, *item ) );
    item->SetBold( false );
    if( const KnowledgeGroup_ABC* kg = dynamic_cast< const KnowledgeGroup_ABC* >( &entity ) )
    {
        const Entity_ABC& top = kg->Get< CommunicationHierarchies >().GetTop();
        const CommunicationHierarchies& teamHierarchy = top.Get< CommunicationHierarchies >();
        tools::Iterator< const Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const Entity_ABC* entity = &it.NextElement();
            if( dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
            {
                item->SetBold( kg == entity );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::CanChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
bool CommunicationListView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    const Automat_ABC*        automat   = dynamic_cast< const Automat_ABC* >       ( &entity );
    const Formation_ABC*      formation = dynamic_cast< const Formation_ABC* >     ( &entity );
    const Ghost_ABC*          ghost     = dynamic_cast< const Ghost_ABC* >         ( &entity );
    const KnowledgeGroup_ABC* group     = dynamic_cast< const KnowledgeGroup_ABC* >( &superior );
    if( ghost && ghost->GetGhostType() != eGhostType_Automat )
        return false;
    if( ( automat || ghost || formation ) && group )
        return &entity.Get< TacticalHierarchies >().GetTop() == &superior.Get< CommunicationHierarchies >().GetTop();
    else if( const KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const KnowledgeGroup_ABC* >( &entity ) )
    {
        const Entity_ABC* com = &knowledgegroup->Get< CommunicationHierarchies >().GetTop();
        const Entity_ABC* team = dynamic_cast< const Entity_ABC* >( &superior );
        if( com && com == team )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::DoChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void CommunicationListView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    Drop( entity, superior );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::OnChangeKnowledgeGroup
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void CommunicationListView::OnChangeKnowledgeGroup()
{
    if( gui::ValuedListItem* valuedItem = static_cast< gui::ValuedListItem* >( selectedItem() ) )
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( !changeSuperiorDialog_ )
            changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, true );
        changeSuperiorDialog_->Show( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const Team_ABC& /*team*/, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationListView", "Create knowledge group" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const kernel::Automat_ABC& /*agent*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ) );
}

namespace
{
    void Update( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& group, bool& result )
    {
        if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* > ( &entity ) )
        {
            kernel::CommunicationHierarchies& hierarchy = const_cast< kernel::CommunicationHierarchies& >( automat->Get< kernel::CommunicationHierarchies >() );
            if( &hierarchy.GetTop() == &group.Get< kernel::CommunicationHierarchies >().GetTop() )
            {
                static_cast< AutomatCommunications& >( hierarchy ).ChangeSuperior( const_cast< Entity_ABC& >( group ) );
                result = true;
            }
        }
        else if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* > ( &entity ) )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = formation->Get< TacticalHierarchies >().CreateSubordinateIterator();
            while( children.HasMoreElements() )
                Update( children.NextElement(), group, result );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Drop
// Created: SBO 2006-09-26
// LTO
// -----------------------------------------------------------------------------
bool CommunicationListView::Drop( const Entity_ABC& draggedEntity, const Entity_ABC& target )
{
    const Automat_ABC*        automat   = dynamic_cast< const Automat_ABC* >       ( &draggedEntity );
    const Ghost_ABC*          ghost     = dynamic_cast< const Ghost_ABC* >         ( &draggedEntity );
    const Formation_ABC*      formation = dynamic_cast< const Formation_ABC* >     ( &draggedEntity );
    const KnowledgeGroup_ABC* group     = dynamic_cast< const KnowledgeGroup_ABC* >( &target );
    if( ghost && ghost->GetGhostType() != eGhostType_Automat )
        return false;
    if( ( automat || formation || ghost ) && group )
    {
        // moving an automat or ghost or formation under knowledgegroup
        bool result = false;
        ::Update( draggedEntity, target, result );
        return result;
    }
    else
    {
        // moving a knowledgegroup under knowledgegroup
        const KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const KnowledgeGroup_ABC* >( &draggedEntity );
        const Entity_ABC* team = dynamic_cast< const Entity_ABC* >( &target );
        if( knowledgegroup && team )
        {
            // moving knowledgegroup under side
            CommunicationHierarchies& com = const_cast< CommunicationHierarchies& >( knowledgegroup->Get< CommunicationHierarchies >() );
            if( &com.GetTop() != team )
                return false;
            static_cast< KnowledgeGroupCommunications& >( com ).ChangeSuperior( const_cast< Entity_ABC& >( *team ) );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::keyPressEvent
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void CommunicationListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        modelBuilder_.DeleteEntity( *static_cast< gui::ValuedListItem* >( selectedItem() )->GetValue< const Entity_ABC >() );
    else
        Q3ListView::keyPressEvent( event );
}