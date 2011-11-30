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

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor constructor
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
MultipleResolverEditor< Entity, Resolver >::MultipleResolverEditor( QDialog*& self, QWidget* parent, const Resolver& resolver )
    : QDialog( parent, "MultipleResolverEditor", true )
    , self_       ( self )
    , values_     ( 0 )
{
    setMinimumSize( 250, 250 );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    // List
    listBox_ = new Q3ListBox();
    listBox_->setSelectionMode( Q3ListBox::Multi );
    tools::Iterator< const Entity& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity& entity = it.NextElement();
        listBox_->insertItem( entity.GetName().c_str() );
        entities_.push_back( &entity );
    }
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton( tr( "Ok" ) );
    QPushButton* cancel = new QPushButton( tr( "Cancel" ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    // Layouts
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancel );
    mainLayout->addWidget( listBox_, 1 );
    mainLayout->addLayout( buttonLayout );
    show();
}

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor destructor
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
MultipleResolverEditor< Entity, Resolver >::~MultipleResolverEditor()
{
    self_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MultipleResolverEditor::SetCurrentItem
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
template< typename Entity, typename Resolver >
void MultipleResolverEditor< Entity, Resolver >::SetCurrentItem( std::vector< Entity* >& entity )
{
    values_ = &entity;
    for( unsigned int i = 0; i < listBox_->count(); ++i )
        listBox_->setSelected( i, std::find( entity.begin(), entity.end(), entities_[i] ) != entity.end() );
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
    for( unsigned int i = 0; i < listBox_->count(); ++i )
        if( listBox_->isSelected( i ) )
            values_->push_back( const_cast< Entity* >( entities_[i] ) );
    QDialog::accept();
}
