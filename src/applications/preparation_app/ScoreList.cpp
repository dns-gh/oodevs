// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreList.h"
#include "moc_ScoreList.cpp"
#include "ScoreEditor.h"
#include "clients_kernel/Controllers.h"
#include "preparation/Score_ABC.h"

// -----------------------------------------------------------------------------
// Name: ScoreList constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::ScoreList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Resolver_ABC< IndicatorPrimitive, QString >& primitives )
    : QVBox( parent )
    , controllers_( controllers )
    , factory_( factory )
    , scores_( new gui::ListDisplayer< ScoreList >( this, *this, factory ) )
    , editor_( new ScoreEditor( this, controllers, factory, primitives ) )
{
    layout()->setAlignment( Qt::AlignRight );
    scores_->AddColumn( tr( "Name" ) );
    {
        QHBox* box = new QHBox( this );
        editButton_ = new QPushButton( tr( "Edit..." ), box );
        deleteButton_ = new QPushButton( tr( "Delete" ), box );
        connect( editButton_, SIGNAL( clicked() ), SLOT( OnEdit() ) );
        connect( deleteButton_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    connect( scores_, SIGNAL( doubleClicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnEdit() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreList destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreList::~ScoreList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnEdit
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnEdit()
{
    if( Score_ABC* score = FindSelected() )
        editor_->StartEdit( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::OnDelete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::OnDelete()
{
    // $$$$ SBO 2009-04-20: prompt confirmation
    if( const Score_ABC* score = FindSelected() )
        emit ScoreDeleted( *score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyCreated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyCreated( const Score_ABC& element )
{
    gui::ValuedListItem* item = factory_.CreateItem( scores_ );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyUpdated
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyUpdated( const Score_ABC& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::NotifyDeleted
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::NotifyDeleted( const Score_ABC& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        scores_->RemoveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::Display
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreList::Display( const Score_ABC& score, gui::ValuedListItem* item )
{
    item->SetNamed( score );
}

// -----------------------------------------------------------------------------
// Name: ScoreList::FindSelected
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score_ABC* ScoreList::FindSelected() const
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( scores_->selectedItem() ) )
        return item->GetValue< Score_ABC >();
    return 0;
}
