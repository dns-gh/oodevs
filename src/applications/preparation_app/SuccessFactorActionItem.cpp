// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorActionItem.h"
#include "moc_SuccessFactorActionItem.cpp"
#include "clients_gui/resources.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/ValuedComboBox.h"
#include "preparation/SuccessFactorAction.h"
#include "preparation/SuccessFactorActions.h"
#include "preparation/SuccessFactorActionParameter.h"
#include "preparation/SuccessFactorActionType.h"
#include "preparation/SuccessFactorActionTypes.h"
#include "preparation/SuccessFactorActionTypeParameter.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActionItem::SuccessFactorActionItem( const QString& objectName, const SuccessFactorActionTypes& actions )
    : parameters_( 0 )
{
    gui::SubObjectName subObject( objectName );
    setObjectName( objectName );
    setFrameStyle( QFrame::Panel | QFrame::Raised );
    QHBoxLayout* box = new QHBoxLayout();
    box->setSpacing( 5 );
    type_ = new gui::ValuedComboBox< const SuccessFactorActionType* >( "type" );
    tools::Iterator< const SuccessFactorActionType& > it( actions.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const SuccessFactorActionType& type = it.NextElement();
        type_->AddItem( type.GetName(), &type );
    }
    connect( type_, SIGNAL( activated( int ) ), SLOT( OnTypeChanged() ) );
    gui::RichPushButton* addButton = new gui::RichPushButton( "add", "+" );
    addButton->setMaximumWidth( 30 );
    deleteButton_ = new gui::RichPushButton( "delete", MAKE_PIXMAP( trash ), "" );
    deleteButton_->setMaximumWidth( 30 );
    connect( addButton, SIGNAL( clicked() ), SIGNAL( Add() ) );
    connect( deleteButton_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    box->addWidget( type_ );
    box->addWidget( addButton );
    box->addWidget( deleteButton_ );
    layout_ = new QVBoxLayout;
    setLayout( layout_ );
    layout_->setSpacing( 2 );
    layout_->addLayout( box );
    OnTypeChanged();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorActionItem::~SuccessFactorActionItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::StartEdit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::StartEdit( const SuccessFactorAction& action )
{
    type_->SetCurrentItem( &action.GetType() );
    OnTypeChanged();
    tools::Iterator< const SuccessFactorActionParameter& > it( action.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const SuccessFactorActionParameter& parameter = it.NextElement();
        SetParameter( parameter.GetName(), parameter.GetValue() );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::CommitTo
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::CommitTo( SuccessFactorActions& actions ) const
{
    if( const SuccessFactorActionType* type = type_->GetValue() )
    {
        SuccessFactorAction* action = new SuccessFactorAction( *type );
        BOOST_FOREACH( const T_ParameterEditors::value_type editor, editors_ )
            action->AddParameter( editor.first, editor.second->text() );
        actions.AddAction( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::EnableDeletion
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::EnableDeletion( bool enable )
{
    deleteButton_->setEnabled( enable );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::OnTypeChanged
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::OnTypeChanged()
{
    editors_.clear();
    delete parameters_;
    parameters_ = new gui::RichGroupBox( "parameters" + objectName() );
    layout_->addWidget( parameters_ );
    QHBoxLayout* parametersLayout = new QHBoxLayout( parameters_ );
    parametersLayout->setMargin( 5 );
    parameters_->setFlat( true );
    if( const SuccessFactorActionType* type = type_->GetValue() )
    {
        tools::Iterator< const SuccessFactorActionTypeParameter& > it( type->CreateIterator() );
        while( it.HasMoreElements() )
        {
            const SuccessFactorActionTypeParameter& parameter = it.NextElement();
            AddParameter( parameter.GetName(), parameter.GetDefault() );
        }
    }
    parameters_->setVisible( !editors_.empty() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::AddParameter
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::AddParameter( const QString& name, const QString& value )
{
    QLabel* label = new QLabel( QString( "%1: " ).arg( name ) );
    editors_[ name ] = new gui::RichLineEdit( "name", value );
    parameters_->layout()->addWidget( label );
    parameters_->layout()->addWidget( editors_[ name ] );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::SetParameter
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::SetParameter( const QString& name, const QString& value )
{
    auto it = editors_.find( name );
    if( it != editors_.end() )
        it->second->setText( value );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorActionItem::OnDelete
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorActionItem::OnDelete()
{
    emit Deleted( *this );
}
