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
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "preparation/SuccessFactor.h"

Q_DECLARE_METATYPE( const SuccessFactor* )

// -----------------------------------------------------------------------------
// Name: SuccessFactorList constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorList::SuccessFactorList( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores, SuccessFactorsModel& success )
    : controllers_( controllers )
    , factors_( new gui::RichWidget< QTreeWidget >( "factors" ) )
    , editor_( new SuccessFactorEditor( parent, controllers_, actionTypes, scores, success ) )
{
    gui::SubObjectName subObject( "SuccessFactorList" );
    setAlignment( Qt::AlignRight );
    factors_->setHeaderLabels( QStringList( tr( "Name" ) ) );
    factors_->setRootIsDecorated( false );
    addWidget( factors_ );
    {
        QHBoxLayout* box = new QHBoxLayout();
        editButton_ = new gui::RichPushButton( "edit", tr( "Edit..." ) );
        deleteButton_ = new gui::RichPushButton( "delete", tr( "Delete" ) );
        box->addWidget( editButton_ );
        box->addWidget( deleteButton_ );
        connect( editButton_, SIGNAL( clicked() ), SLOT( OnEdit() ) );
        connect( deleteButton_, SIGNAL( clicked() ), SLOT( OnDelete() ) );
        addLayout( box );
    }
    connect( factors_, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), SLOT( OnEdit() ) );
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
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText( 0, element.GetName() );
    item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( &element ) );
    factors_->addTopLevelItem( item );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::NotifyUpdated
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::NotifyUpdated( const SuccessFactor& element )
{
    for( int i = 0; i < factors_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = factors_->topLevelItem( i );
        if( item->data( 0, Qt::UserRole + 1 ).value< const SuccessFactor* >() == &element )
            item->setText( 0, element.GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorList::NotifyDeleted( const SuccessFactor& element )
{
    for( int i = 0; i < factors_->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = factors_->topLevelItem( i );
        if( item->data( 0, Qt::UserRole + 1 ).value< const SuccessFactor* >() == &element )
        {
            delete item;
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorList::FindSelected
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactor* SuccessFactorList::FindSelected() const
{
    if( QTreeWidgetItem* selected = factors_->currentItem() )
        return const_cast< SuccessFactor* >( selected->data( 0, Qt::UserRole + 1 ).value< const SuccessFactor* >() );
    return 0;
}
