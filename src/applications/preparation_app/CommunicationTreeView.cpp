// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CommunicationTreeView.h"
#include "moc_CommunicationTreeView.cpp"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/AutomatCommunications.h"
#include "preparation/KnowledgeGroupCommunications.h" // LTO

// TODO factoriser avec gaming

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView constructor
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
CommunicationTreeView::CommunicationTreeView( const QString& objectName,
                                              kernel::Controllers& controllers,
                                              const kernel::Profile_ABC& profile,
                                              gui::ModelObserver_ABC& modelObserver,
                                              const gui::EntitySymbols& symbols,
                                              gui::ChangeSuperiorDialog& changeSuperiorDialog,
                                              QWidget* parent )
    : gui::HierarchyTreeView< kernel::CommunicationHierarchies >( objectName, controllers, profile, modelObserver, symbols, parent )
    , changeSuperiorDialog_( changeSuperiorDialog )
{
    connect( this,                   SIGNAL( SelectionChanged( const std::vector< const kernel::Entity_ABC* >& ) ),
             &changeSuperiorDialog_, SLOT( OnKnowledgeGroupSelectionChanged( const std::vector< const kernel::Entity_ABC* >& ) ) );
    changeSuperiorDialog_.SetKnowledgeGroupFunctors(
        boost::bind( &CommunicationTreeView::CanChangeSuperior, this, _1, _2 ),
        [&] ( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) {
            if( auto automat = dynamic_cast< kernel::Automat_ABC* >( &entity ) )
                Drop( *automat, superior );
            else if( auto formation = dynamic_cast< kernel::Formation_ABC* >( &entity ) )
                Drop( *formation, superior );
            else if( auto ghost = dynamic_cast< kernel::Ghost_ABC* >( &entity ) )
                Drop( *ghost, superior );
            else if( auto kg = dynamic_cast< kernel::KnowledgeGroup_ABC* >( &entity ) )
                Drop( *kg, superior );
    } );
    SetLessThanEntityFunctor( &tools::LessThanById );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView destructor
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
CommunicationTreeView::~CommunicationTreeView()
{
    controllers_.Unregister( *this );
}

bool CommunicationTreeView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    auto automat   = dynamic_cast< const kernel::Automat_ABC* >       ( &entity );
    auto formation = dynamic_cast< const kernel::Formation_ABC* >     ( &entity );
    auto ghost     = dynamic_cast< const kernel::Ghost_ABC* >         ( &entity );
    auto group     = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &superior );
    if( ghost && ghost->GetGhostType() != eGhostType_Automat )
        return false;
    if( ( automat || ghost || formation ) && group )
        return &entity.Get< kernel::TacticalHierarchies >().GetTop() == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    else if( const kernel::KnowledgeGroup_ABC* knowledgegroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
    {
        const kernel::Entity_ABC* com = &knowledgegroup->Get< kernel::CommunicationHierarchies >().GetTop();
        const kernel::Entity_ABC* team = dynamic_cast< const kernel::Entity_ABC* >( &superior );
        if( com && com == team )
            return true;
        else if( group && ( knowledgegroup != group ) )
            return com == &superior.Get< kernel::CommunicationHierarchies >().GetTop();
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::OnCreateCommunication
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::OnCreateCommunication()
{
    modelObserver_.CreateCommunication();
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::OnChangeKnowledgeGroup
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::OnChangeKnowledgeGroup()
{
    QModelIndexList list = selectionModel()->selectedIndexes();
    if( list.size() == 1)
        if( auto* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( list.front() ) )
            changeSuperiorDialog_.Show( *entity, tr( "Select new knowledge group" ), gui::ChangeSuperiorDialog::eKnowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyCreated
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyCreated( const kernel::Entity_ABC& entity )
{
    gui::HierarchyTreeView< kernel::CommunicationHierarchies >::ElementObserver_ABC< kernel::Entity_ABC >::NotifyCreated( entity );
    UpdateLongName( entity );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyUpdated
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::HierarchyTreeView< kernel::CommunicationHierarchies >::ElementObserver_ABC< kernel::Entity_ABC >::NotifyUpdated( entity );
    UpdateLongName( entity );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyDeleted
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyDeleted( const kernel::KnowledgeGroup_ABC& kg )
{
    UpdateFonts( kg, true );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyCreated
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyCreated( const kernel::KnowledgeGroup_ABC& kg )
{
    UpdateFonts( kg, false );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::UpdateLongName
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void CommunicationTreeView::UpdateLongName( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
        item->setEditable( !gui::longname::SetItemLongName( entity, *item ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::UpdateFonts
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void CommunicationTreeView::UpdateFonts( const kernel::KnowledgeGroup_ABC& kg, bool deleted )
{
    const kernel::Entity_ABC& top = kg.Get< kernel::CommunicationHierarchies >().GetTop();
    const kernel::CommunicationHierarchies& teamHierarchy = top.Get< kernel::CommunicationHierarchies >();
    tools::Iterator< const kernel::Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
    bool bold = true;
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC* entity = &it.NextElement();
        const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( entity );
        if( group && ( !deleted || group != &kg ) )
        {
            QStandardItem* item = dataModel_.FindDataItem( *entity );
            if( item )
            {
                QFont font = item->font();
                font.setBold( bold );
                item->setFont( font );
            }
            bold = false;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::Team_ABC& /*agent*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationTreeView", "Create knowledge group" ), this, SLOT( OnCreateCommunication() ) );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::Automat_ABC& /*agent*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::KnowledgeGroup_ABC& /*knowledgegroup*/, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Creation", tools::translate( "CommunicationTreeView", "Create sub knowledge group" ), this, SLOT( OnCreateCommunication() ) );
    menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::NotifyContextMenu
// Created: JSR 2014-10-03
// -----------------------------------------------------------------------------
void CommunicationTreeView::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    if( isVisible() && ghost.GetGhostType() == eGhostType_Automat )
        menu.InsertItem( "Command", tr( "Change knowledge group" ), this, SLOT( OnChangeKnowledgeGroup() ), false, 1 );
}

namespace
{
    void Update( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& group )
    {
        if( dynamic_cast< const kernel::Automat_ABC* > ( &entity ) || dynamic_cast< const kernel::Ghost_ABC* > ( &entity ) )
        {
            kernel::CommunicationHierarchies& hierarchy = const_cast< kernel::CommunicationHierarchies& >( entity.Get< kernel::CommunicationHierarchies >() );
            if( &hierarchy.GetTop() == &group.Get< kernel::CommunicationHierarchies >().GetTop() )
                static_cast< AutomatCommunications& >( hierarchy ).ChangeSuperior( const_cast< kernel::Entity_ABC& >( group ) );
        }
        else if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* > ( &entity ) )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = formation->Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            while( children.HasMoreElements() )
                Update( children.NextElement(), group );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target );
    if( group )
        ::Update( item, target );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target );
    if( group )
        ::Update( item, target );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target );
    if( group && item.GetGhostType() == eGhostType_Automat )
        ::Update( item, target );
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::Drop
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void CommunicationTreeView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    kernel::CommunicationHierarchies& com = const_cast< kernel::CommunicationHierarchies& >( item.Get< kernel::CommunicationHierarchies >() );
    if( const kernel::KnowledgeGroup_ABC* group = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &target ) )
    { // moving a knowledgegroup under knowledgegroup
        if( group && group != &item )
        {
            if( &com.GetTop() == &target.Get< kernel::CommunicationHierarchies >().GetTop() )
                static_cast< AutomatCommunications& >( com ).ChangeSuperior( const_cast< kernel::Entity_ABC& >( target ) );
        }
    }
    else if( const kernel::Team_ABC* team = dynamic_cast< const kernel::Team_ABC* >( &target ) )
    { // moving knowledgegroup under side
        if( &com.GetTop() == team )
            static_cast< KnowledgeGroupCommunications& >( com ).ChangeSuperior( const_cast< kernel::Entity_ABC& >( target ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CommunicationTreeView::keyPressEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void CommunicationTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && event->key() == Qt::Key_Delete && index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            modelObserver_.DeleteEntity( *entity );
    }
    else
        gui::HierarchyTreeView< kernel::CommunicationHierarchies >::keyPressEvent( event );
}
