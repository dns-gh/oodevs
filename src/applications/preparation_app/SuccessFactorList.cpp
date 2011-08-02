// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorList.h"
#include "moc_SuccessFactorList.cpp"
#include "SuccessFactorEditor.h"
#include "clients_kernel/Controllers.h"
#include "preparation/SuccessFactor.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorList constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorList::SuccessFactorList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores )
    : Q3VBox( parent )
    , controllers_( controllers )
    , factory_( factory )
    , factors_( new gui::ListDisplayer< SuccessFactorList >( this, *this, factory ) )
    , editor_( new SuccessFactorEditor( this, controllers_, actionTypes, scores ) )
{
    layout()->setAlignment( Qt::AlignRight );
    factors_->AddColumn( tr( "Name" ) );
    {
        Q3HBox* box = new Q3HBox( this );
        editButton_ = new QPushButton( tr( "Edit..." ), box );
        deleteButton_ = new QPushButton( tr( "Delete" ), box );
        connect( editButton_, SIGNAL( clicked() ), SLOT( OnEdit() ) );
        connect( deleteButton_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    }
    connect( factors_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnEdit() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorList::~SuccessFactorList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::OnEdit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::OnEdit()
{
    if( SuccessFactor* factor = FindSelected() )
        editor_->StartEdit( *factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::OnDelete
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::OnDelete()
{
    // $$$$ SBO 2009-06-15: Prompt confirmation
    if( const SuccessFactor* factor = FindSelected() )
        emit Deleted( *factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::NotifyCreated
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::NotifyCreated( const SuccessFactor& element )
{
    gui::ValuedListItem* item = factory_.CreateItem( factors_ );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::NotifyUpdated
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::NotifyUpdated( const SuccessFactor& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, factors_->firstChild() ) )
        Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::NotifyDeleted( const SuccessFactor& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, factors_->firstChild() ) )
        factors_->removeItem( item );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::Display
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::Display( const SuccessFactor& factor, gui::ValuedListItem* item )
{
    item->SetNamed( factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::FindSelected
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactor* SuccessFactorList::FindSelected() const
{
    if( gui::ValuedListItem* item = static_cast< gui::ValuedListItem* >( factors_->selectedItem() ) )
        return item->GetValue< SuccessFactor >();
    return 0;
}
