// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "actions/Action_ABC.h"
#include "actions/MaintenancePriorities.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/resources.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QObject* parent, const kernel::OrderParameter& parameter, const tools::Resolver< kernel::EquipmentType >& resolver )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , resolver_ ( resolver )
    , list_     ( 0 )
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
    Q3ListView* CreateList( QWidget* parent )
    {
        Q3ListView* list = new Q3ListView( parent );
        list->addColumn( "" );
        list->header()->hide();
        list->setResizeMode( Q3ListView::LastColumn );
        list->setAllColumnsShowFocus( true );
        return list;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamEquipmentList::BuildInterface( QWidget* parent )
{
    Q3HBox* hBox = new Q3HBox( parent );
    {
        baseList_ = CreateList( hBox );
        baseList_->setSorting( 0, true );
        tools::Iterator< const kernel::EquipmentType& > it( resolver_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::EquipmentType& type = it.NextElement();
            ::gui::ValuedListItem* item = new ::gui::ValuedListItem( baseList_ );
            item->SetNamed( type );
        }
        Q3VBox* buttonBox = new Q3VBox( hBox );
        buttonBox->layout()->setAlignment( Qt::AlignVCenter );
        QPushButton* addBtn = new QPushButton( MAKE_ICON( right_arrow ), QString::null, buttonBox );
        addBtn->setFixedSize( 32, 32 );
        QPushButton* removeBtn = new QPushButton( MAKE_ICON( left_arrow ), QString::null, buttonBox );
        removeBtn->setFixedSize( 32, 32 );

        connect( addBtn, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( baseList_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnAdd() ) );
        connect( removeBtn, SIGNAL( clicked() ), SLOT( OnRemove() ) );
    }
    {
        list_ = CreateList( hBox );
        list_->setSorting( -1, true );
        Q3VBox* buttonBox = new Q3VBox( hBox );
        buttonBox->layout()->setAlignment( Qt::AlignVCenter );
        QPushButton* upBtn = new QPushButton( MAKE_ICON( arrow_up ), QString::null, buttonBox );
        upBtn->setFixedSize( 32, 32 );
        QPushButton* downBtn = new QPushButton( MAKE_ICON( arrow_down ), QString::null, buttonBox );
        downBtn->setFixedSize( 32, 32 );
        connect( list_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnRemove() ) );
        connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
        connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    }
    return hBox;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::MaintenancePriorities > param( new actions::parameters::MaintenancePriorities( parameter_ ) );
    for( Q3ListViewItemIterator it( list_ ); it.current(); ++it )
        param->AddPriority( *static_cast< const ::gui::ValuedListItem* >( it.current() )->GetValue< kernel::EquipmentType >() );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnUp()
{
    Q3ListViewItem* item = list_->currentItem();
    if( item && item->itemAbove() )
        item->itemAbove()->moveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnDown()
{
    Q3ListViewItem* item = list_->currentItem();
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
void ParamEquipmentList::Move( Q3ListView* from, Q3ListView* to )
{
    ::gui::ValuedListItem* item = static_cast< ::gui::ValuedListItem* >( from->selectedItem() );
    if( item )
    {
        ::gui::ValuedListItem* newItem = new ::gui::ValuedListItem( to );
        newItem->SetNamed( *item->GetValue< const kernel::EquipmentType >() );
        delete item;
    }
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamEquipmentList::IsOptional() const
{
    return parameter_.IsOptional();
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CheckValidity
// Created: LGY 2010-08-06
// -----------------------------------------------------------------------------
bool ParamEquipmentList::CheckValidity()
{
    return true;
}
