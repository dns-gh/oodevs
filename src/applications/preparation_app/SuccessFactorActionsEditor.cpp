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
SuccessFactorActionsEditor::SuccessFactorActionsEditor( const QString& objectName, const SuccessFactorActionTypes& actionTypes )
    : actionTypes_( actionTypes )
    , scrollArea_( 0 )
{
    gui::ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActionsEditor::~SuccessFactorActionsEditor()
{
    gui::ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionsEditor::StartEdit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorActionsEditor::StartEdit( const SuccessFactorActions& actions )
{
    items_.clear();
    delete scrollArea_;
    QVBoxLayout* scrollLayout = new QVBoxLayout;
    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable( true );
    QWidget* widget = new QWidget;
    widget->setLayout( scrollLayout );
    scrollArea_->setWidget( widget );
    scrollLayout->setMargin( 2 );
    scrollLayout->setSpacing( 2 );
    scrollArea_->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea_->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    addWidget( scrollArea_ );
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
    gui::SubObjectName subObject( "SuccessFactorActionsEditor" );
    SuccessFactorActionItem* item = new SuccessFactorActionItem( "SuccessFactorActionItem" + QString::number( items_.size() ), actionTypes_ );
    scrollArea_->widget()->layout()->addWidget( item );
    items_.push_back( item );
    items_.front()->EnableDeletion( items_.size() > 1 );
    connect( item, SIGNAL( Add() ), SLOT( CreateItem() ) );
    connect( item, SIGNAL( Deleted( SuccessFactorActionItem& ) ), SLOT( OnDelete( SuccessFactorActionItem& ) ) );
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
        scrollArea_->widget()->layout()->removeWidget( &item );
    }
}
