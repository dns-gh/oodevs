// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "MultipleResolverEditor.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor constructor
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
MultipleResolverEditor< Entity, Resolver >::MultipleResolverEditor( const QString& objectName, QWidget* parent, const Resolver& resolver )
    : gui::PropertyDialog( parent )
    , values_( 0 )
{
    gui::SubObjectName subObject( objectName );
    setMinimumSize( 250, 250 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    // List
    listBox_ = new gui::RichWidget< QListWidget >( "listBox" );
    listBox_->setSelectionMode( QAbstractItemView::MultiSelection );
    tools::Iterator< const Entity& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity& entity = it.NextElement();
        listBox_->addItem( entity.GetName().c_str() );
        entities_.push_back( &entity );
    }
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    gui::RichPushButton* okButton = new gui::RichPushButton( "ok", tr( "Ok" ) );
    gui::RichPushButton* cancel = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    // Layouts
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancel );
    mainLayout->addWidget( listBox_, 1 );
    mainLayout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor destructor
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
MultipleResolverEditor< Entity, Resolver >::~MultipleResolverEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor::SetCurrentItem
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
void MultipleResolverEditor< Entity, Resolver >::SetCurrentItem( std::vector< Entity* >& entity )
{
    values_ = &entity;
    for( int i = 0; i < listBox_->count(); ++i )
        listBox_->item( i )->setSelected( std::find( entity.begin(), entity.end(), entities_[i] ) != entity.end() );
}

// -----------------------------------------------------------------------------
// Name: std::vector< Entity* > MultipleResolverEditor::GetValue
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
std::vector< Entity* > MultipleResolverEditor< Entity, Resolver >::GetValue()
{
    return *values_;
}

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor::OnAccept
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
void MultipleResolverEditor< Entity, Resolver >::accept()
{
    values_->clear();
    for( int i = 0; i < listBox_->count(); ++i )
        if( listBox_->item( i )->isSelected() )
            values_->push_back( const_cast< Entity* >( entities_[i] ) );
    QDialog::accept();
}
