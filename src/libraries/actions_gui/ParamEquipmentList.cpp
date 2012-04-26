// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/MaintenancePriorities.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_gui/resources.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , resolver_( builder.GetStaticModel().objectTypes_ )
    , builder_ ( builder )
    , list_    ( 0 )
    , baseList_( 0 )
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
    Param_ABC::BuildInterface( parent );
    kernel::MaintenanceStates_ABC* maintenance =  builder_.GetCurrentEntity().Retrieve< kernel::MaintenanceStates_ABC >();
    QGridLayout* layout = new QGridLayout( group_ );
    {
        baseList_ = CreateList( parent );
        baseList_->setSorting( 0, true );
        tools::Iterator< const kernel::EquipmentType& > it( resolver_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::EquipmentType& type = it.NextElement();
            if( !maintenance || !maintenance->HasPriority( &type ) )
            {
                ::gui::ValuedListItem* item = new ::gui::ValuedListItem( baseList_ );
                item->SetNamed( type );
            }
        }

        QPushButton* addBtn = new QPushButton( MAKE_ICON( right_arrow ), QString::null, parent );
        addBtn->setFixedSize( 32, 32 );
        QPushButton* removeBtn = new QPushButton( MAKE_ICON( left_arrow ), QString::null, parent );
        removeBtn->setFixedSize( 32, 32 );

        layout->addWidget( baseList_, 0, 0, 2, 1 );
        layout->addWidget( addBtn, 0, 1 );
        layout->addWidget( removeBtn, 1, 1 );

        connect( addBtn, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( baseList_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnAdd() ) );
        connect( removeBtn, SIGNAL( clicked() ), SLOT( OnRemove() ) );
    }
    {
        list_ = CreateList( parent );
        list_->setSorting( -1, true );

        if( maintenance )
        {
            std::vector< const kernel::EquipmentType* > priorities = maintenance->GetPriorities();
            for( std::vector< const kernel::EquipmentType* >::const_reverse_iterator it = priorities.rbegin(); it != priorities.rend(); ++it )
            {
                ::gui::ValuedListItem* item = new ::gui::ValuedListItem( list_ );
                item->SetNamed( **it );
            }
        }

        QPushButton* upBtn = new QPushButton( MAKE_ICON( arrow_up ), QString::null, parent );
        upBtn->setFixedSize( 32, 32 );
        QPushButton* downBtn = new QPushButton( MAKE_ICON( arrow_down ), QString::null, parent );
        downBtn->setFixedSize( 32, 32 );

        layout->addWidget( list_, 0, 2, 2, 1 );
        layout->addWidget( upBtn, 0, 3 );
        layout->addWidget( downBtn, 1, 3 );

        connect( list_, SIGNAL( doubleClicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnRemove() ) );
        connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
        connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    }
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    std::auto_ptr< actions::parameters::MaintenancePriorities > param( new actions::parameters::MaintenancePriorities( parameter_ ) );
    if( IsChecked() )
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
// Name: ParamEquipmentList::InternalCheckValidity
// Created: LGY 2010-08-06
// -----------------------------------------------------------------------------
bool ParamEquipmentList::InternalCheckValidity() const
{
    return true;
}
