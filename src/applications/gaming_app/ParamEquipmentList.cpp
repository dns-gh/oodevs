// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "clients_kernel/EquipmentType.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterMaintenancePriorities.h"
#include "icons.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QObject* parent, const OrderParameter& parameter, const Resolver< EquipmentType >& resolver )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , resolver_( resolver )
    , list_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamEquipmentList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::~ParamEquipmentList()
{
    // NOTHING
}

namespace
{
    QListView* CreateList( QWidget* parent )
    {
        QListView* list = new QListView( parent );
        list->addColumn( "" );
        list->header()->hide();
        list->setResizeMode( QListView::ResizeMode::LastColumn );
        list->setAllColumnsShowFocus( true );
        return list;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamEquipmentList::BuildInterface( QWidget* parent )
{
    QHBox* hBox = new QHBox( parent );
    {
        baseList_ = CreateList( hBox );
        baseList_->setSorting( 0, true );
        Iterator< const EquipmentType& > it( resolver_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const EquipmentType& type = it.NextElement();
            ValuedListItem* item = new ValuedListItem( baseList_ );
            item->SetNamed( type );
        }

        QVBox* buttonBox = new QVBox( hBox );
        buttonBox->layout()->setAlignment( Qt::AlignVCenter );
        QPushButton* addBtn = new QPushButton( MAKE_ICON( right_arrow ), QString::null, buttonBox );
        addBtn->setFixedSize( 32, 32 );
        QPushButton* removeBtn = new QPushButton( MAKE_ICON( left_arrow ), QString::null, buttonBox );
        removeBtn->setFixedSize( 32, 32 );

        connect( addBtn, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( removeBtn, SIGNAL( clicked() ), SLOT( OnRemove() ) );
    }

    {
        list_ = CreateList( hBox );
        list_->setSorting( -1, true );

        QVBox* buttonBox = new QVBox( hBox );
        buttonBox->layout()->setAlignment( Qt::AlignVCenter );
        QPushButton* upBtn = new QPushButton( MAKE_ICON( arrow_up ), QString::null, buttonBox );
        upBtn->setFixedSize( 32, 32 );
        QPushButton* downBtn = new QPushButton( MAKE_ICON( arrow_down ), QString::null, buttonBox );
        downBtn->setFixedSize( 32, 32 );

        connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
        connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( ActionParameterContainer_ABC& action ) const
{
    std::auto_ptr< ActionParameterMaintenancePriorities > param( new ActionParameterMaintenancePriorities( parameter_ ) );
    for( QListViewItemIterator it( list_ ); it.current(); ++it )
        param->AddPriority( *static_cast< const ValuedListItem* >( it.current() )->GetValue< EquipmentType >() );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnUp()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemAbove() )
        item->itemAbove()->moveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnDown()
{
    QListViewItem* item = list_->currentItem();
    if( item && item->itemBelow() )
        item->moveItem( item->itemBelow() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnAdd
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnAdd()
{
    Move( baseList_, list_ );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnRemove
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnRemove()
{
    Move( list_, baseList_ );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::Move
// Created: AGE 2007-10-24
// -----------------------------------------------------------------------------
void ParamEquipmentList::Move( QListView* from, QListView* to )
{
    ValuedListItem* item = static_cast< ValuedListItem* >( from->selectedItem() );
    if( item )
    {
        ValuedListItem* newItem = new ValuedListItem( to );
        newItem->SetNamed( *item->GetValue< const EquipmentType >() );
        delete item;
    }
}
