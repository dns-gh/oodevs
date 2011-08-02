// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorActionsEditor.h"
#include "moc_SuccessFactorActionsEditor.cpp"
#include "SuccessFactorActionItem.h"
#include "preparation/SuccessFactorActions.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActionsEditor::SuccessFactorActionsEditor( QWidget* parent, const SuccessFactorActionTypes& actionTypes )
    : Q3ScrollView( parent )
    , mainWidget_( 0 )
    , actionTypes_( actionTypes )
{
    setHScrollBarMode( Q3ScrollView::AlwaysOff );
    setResizePolicy( Q3ScrollView::AutoOneFit );
    setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActionsEditor::~SuccessFactorActionsEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor::StartEdit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorActionsEditor::StartEdit( const SuccessFactorActions& actions )
{
    items_.clear();
    delete mainWidget_;
    mainWidget_ = new Q3VBox( viewport() );
    addChild( mainWidget_ );
    tools::Iterator< const SuccessFactorAction& > it( actions.CreateIterator() );
    while( it.HasMoreElements() )
        CreateItem()->StartEdit( it.NextElement() );
    if( items_.empty() )
        CreateItem();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor::CommitTo
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorActionsEditor::CommitTo( SuccessFactorActions& actions ) const
{
    actions.DeleteAll();
    BOOST_FOREACH( const T_Items::value_type item, items_ )
        item->CommitTo( actions );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor::CreateItem
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorActionItem* SuccessFactorActionsEditor::CreateItem()
{
    SuccessFactorActionItem* item = new SuccessFactorActionItem( mainWidget_, actionTypes_ );
    addChild( mainWidget_ );
    items_.push_back( item );
    items_.front()->EnableDeletion( items_.size() > 1 );
    connect( item, SIGNAL( Add() ), SLOT( CreateItem() ) );
    connect( item, SIGNAL( Deleted( SuccessFactorActionItem& ) ), SLOT( OnDelete( SuccessFactorActionItem& ) ) );
    item->show();
    return item;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor::OnDelete
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionsEditor::OnDelete( SuccessFactorActionItem& item )
{
    if( items_.size() > 1 )
    {
        T_Items::iterator it = std::find( items_.begin(), items_.end(), &item );
        if( it != items_.end() )
            items_.erase( it );
        item.deleteLater();
        if( items_.size() == 1 )
            items_.front()->EnableDeletion( false );
        addChild( mainWidget_ );
    }
}
