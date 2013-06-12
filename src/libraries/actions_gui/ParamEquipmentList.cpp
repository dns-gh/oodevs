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
#include "InterfaceBuilder_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/MaintenancePriorities.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/resources.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"

Q_DECLARE_METATYPE( const kernel::EquipmentType* )

#define EquipmentRole ( Qt::UserRole + 1 )

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

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamEquipmentList::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QGridLayout* layout = new QGridLayout( group_ );
    {
        baseList_ = new QListWidget( parent );
        baseList_->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
        baseList_->setSortingEnabled( true );
        baseList_->sortItems( Qt::AscendingOrder );

        ::gui::RichPushButton* addBtn = new ::gui::RichPushButton( "addBtn", MAKE_ICON( right_arrow ), QString::null, parent );
        addBtn->setFixedSize( 32, 32 );
        ::gui::RichPushButton* removeBtn = new ::gui::RichPushButton( "removeBtn", MAKE_ICON( left_arrow ), QString::null, parent );
        removeBtn->setFixedSize( 32, 32 );

        layout->addWidget( baseList_, 0, 0, 2, 1 );
        layout->addWidget( addBtn, 0, 1 );
        layout->addWidget( removeBtn, 1, 1 );

        connect( addBtn, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( baseList_, SIGNAL( doubleClicked( const QModelIndex& ) ), SLOT( OnAdd() ) );
        connect( removeBtn, SIGNAL( clicked() ), SLOT( OnRemove() ) );
    }
    {
        list_ = new QListWidget( parent );
        list_->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );

        ::gui::RichPushButton* upBtn = new ::gui::RichPushButton( "upBtn", MAKE_ICON( arrow_up ), QString::null, parent );
        upBtn->setFixedSize( 32, 32 );
        ::gui::RichPushButton* downBtn = new ::gui::RichPushButton( "downBtn", MAKE_ICON( arrow_down ), QString::null, parent );
        downBtn->setFixedSize( 32, 32 );

        layout->addWidget( list_, 0, 2, 2, 1 );
        layout->addWidget( upBtn, 0, 3 );
        layout->addWidget( downBtn, 1, 3 );

        connect( list_, SIGNAL( doubleClicked( const QModelIndex& ) ), SLOT( OnRemove() ) );
        connect( upBtn, SIGNAL( clicked() ), SLOT( OnUp() ) );
        connect( downBtn, SIGNAL( clicked() ), SLOT( OnDown() ) );
    }
    group_->setEnabled( false );
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
        for( int row = 0; row <  list_->count(); ++row )
            param->AddPriority( *list_->item( row )->data( EquipmentRole ).value< const kernel::EquipmentType* >() );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnUp
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnUp()
{
    QListWidgetItem* item = list_->currentItem();
    if( item && list_->count() > 0 && list_->row( item ) > 0 )
    {
        int row = list_->row( item );
        QListWidgetItem* itemTaken = list_->takeItem( list_->row( item ) );
        list_->insertItem( row - 1, itemTaken );
        list_->setCurrentItem( itemTaken );
    }

}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnDown
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnDown()
{
    QListWidgetItem* item = list_->currentItem();
    if( item && list_->count() > 0 && list_->row( item ) < list_->count() )
    {
        int row = list_->row( item );
        QListWidgetItem* itemTaken = list_->takeItem( list_->row( item ) );
        list_->insertItem( row + 1, itemTaken );
        list_->setCurrentItem( itemTaken );
    }
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
void ParamEquipmentList::Move( QListWidget* from, QListWidget* to )
{
    if(! from->selectedItems().isEmpty() )
        if( from->currentItem() )
            to->addItem( from->takeItem( from->row( from->currentItem() ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::InternalCheckValidity
// Created: LGY 2010-08-06
// -----------------------------------------------------------------------------
bool ParamEquipmentList::InternalCheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void ParamEquipmentList::SetEntity( const kernel::Entity_ABC* entity )
{
    if( group_ )
        group_->setEnabled( IsInParam() || entity != 0 );

    if( entity == 0 || baseList_ == 0 || list_ == 0 ) 
        return;

    baseList_->clear();
    list_->clear();

    const kernel::MaintenanceStates_ABC* maintenance = entity->Retrieve< kernel::MaintenanceStates_ABC >();

    auto it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::EquipmentType& type = it.NextElement();
        if( !maintenance || !maintenance->HasPriority( &type ) )
        {
            QListWidgetItem* item = new QListWidgetItem( type.GetName().c_str() );
            item->setData( EquipmentRole, QVariant::fromValue( &type ) );
            baseList_->addItem( item );
        }
    }

    if( maintenance )
    {
        std::vector< const kernel::EquipmentType* > priorities = maintenance->GetPriorities();
        for( auto it = priorities.rbegin(); it != priorities.rend(); ++it )
            list_->addItem( QString( ( **it ).GetName().c_str() ) );
    }
}
