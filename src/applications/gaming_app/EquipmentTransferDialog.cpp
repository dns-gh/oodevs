// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EquipmentTransferDialog.h"
#include "moc_EquipmentTransferDialog.cpp"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/Quantity.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MagicActionType.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/StaticModel.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog constructor
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
EquipmentTransferDialog::EquipmentTransferDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, "EquipmentTransferDialog" )
    , controllers_ ( controllers )
    , static_      ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , selectedFrom_( controllers )
    , selectedTo_  ( controllers )
    , selected_    ( controllers )
    , delegate_    ( new gui::CommonDelegate( this ) )
{
    // Dialog
    setCaption( tr( "Equipment transfer" ) );
    resize( 320, 420 );
    QGridLayout* mainLayout = new QGridLayout( this, 4, 2, 0, 5, "EquipmentTransferDialog_MainLayout" );
    mainLayout->setMargin( 5 );
    // From
    {
        QLabel* titleFrom = new QLabel( tr( "From:" ), this, "EquipmentTransferDialog_FromTitle" );
        labelFrom_ = new QLabel( "---", this, "EquipmentTransferDialog_FromLabel" );
        labelFrom_->setMinimumWidth( 200 );
        labelFrom_->setAlignment( Qt::AlignCenter );
        labelFrom_->setFrameStyle( QFrame::Box | QFrame::Sunken );
        mainLayout->addWidget( titleFrom, 0, 0 );
        mainLayout->addWidget( labelFrom_, 0, 1 );
    }
    // To
    {
        QLabel* titleTo = new QLabel( tr( "To:" ), this, "EquipmentTransferDialog_ToTitle" );
        labelTo_ = new QLabel( "---", this, "EquipmentTransferDialog_ToLabel" );
        labelTo_->setMinimumWidth( 200 );
        labelTo_->setAlignment( Qt::AlignCenter );
        labelTo_->setFrameStyle( QFrame::Box | QFrame::Sunken );
        mainLayout->addWidget( titleTo, 1, 0 );
        mainLayout->addWidget( labelTo_, 1, 1 );
    }
    // Equipments
    {
        equipmentTable_ = new QTableWidget();
        equipmentTable_->setColumnCount( 2 );
        equipmentTable_->setItemDelegate( delegate_ );
        QStringList headers;
        headers << tr( "Equipment" ) << tr( "Quantity" );
        equipmentTable_->setHorizontalHeaderLabels( headers );
        equipmentTable_->horizontalHeader()->setStretchLastSection( false );
        equipmentTable_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
        equipmentTable_->horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
        equipmentTable_->verticalHeader()->hide();
        equipmentTable_->setShowGrid( false );
        mainLayout->addMultiCellWidget( equipmentTable_, 2, 2, 0, 1 );
    }
    // ok / cancel butons
    {
        Q3HBox* buttonLayout = new Q3HBox( this, "EquipmentTransferDialog_ButtonLayout" );
        okButton_ = new QPushButton( tr( "Ok" ), buttonLayout, "EquipmentTransferDialog_ButtonOK" );
        QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout, "EquipmentTransferDialog_ButtonCancel" );
        okButton_->setDefault( true );
        connect( okButton_   , SIGNAL( clicked() ), SLOT( Validate() ) );
        connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
        mainLayout->addMultiCellWidget( buttonLayout, 3, 3, 0, 1 );
    }
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog destructor
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
EquipmentTransferDialog::~EquipmentTransferDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::closeEvent
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::closeEvent( QCloseEvent* /*e*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::Reject
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::Reject()
{
    reject();
    selectedFrom_ = 0;
    selectedTo_ = 0;
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::Show
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::Show()
{
    if( !selected_ )
        return;
    selectedFrom_ = selected_;
    labelFrom_->setText( selectedFrom_->GetName() );
    InitializeEquipments();
    show();
    BeforeSelection();
    AfterSelection();
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::BeforeSelection
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::BeforeSelection()
{
    selectedTo_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::AfterSelection
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::AfterSelection()
{
    labelTo_->setText( selectedTo_ ? selectedTo_->GetName() : "---" );
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::Select
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::Select( const kernel::Agent_ABC& agent )
{
    if( !selectedFrom_ )
        return;
    const kernel::TacticalHierarchies& hierarchyFrom = selectedFrom_->Get< kernel::TacticalHierarchies >();
    const kernel::TacticalHierarchies& hierarchyTo = agent.Get< kernel::TacticalHierarchies >();
    if( &agent != selectedFrom_.ConstCast() && hierarchyFrom.GetTop().GetId() == hierarchyTo.GetTop().GetId() )
        selectedTo_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::NotifyContextMenu
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( agent ) )
    {
        selected_ = &agent;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
        subMenu->insertItem( tr( "Equipment transfer" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::InitializeEquipments
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::InitializeEquipments()
{
    const Equipments* equipments = selectedFrom_->Retrieve< Equipments >();
    if( !equipments )
        throw MASA_EXCEPTION( "Cannot find Equipments extension.");
    equipmentTable_->clearContents();
    equipmentTable_->setRowCount( 0 );
    equipmentIdMap_.clear();
    tools::Iterator< const Equipment& > it = equipments->CreateIterator();
    int row = 0;
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        if( equipment.available_ == 0 )
            continue;
        equipmentTable_->setRowCount( row + 1 );
        equipmentTable_->setItem( row, 0, new QTableWidgetItem() );
        equipmentTable_->setItem( row, 1, new QTableWidgetItem() );
        equipmentIdMap_[ equipment.GetName() ] = equipment.type_.GetId();
        delegate_->AddSpinBox( row, row, 1, 1, 0, equipment.available_ );
        equipmentTable_->item( row, 0 )->setText( equipment.GetName() );
        equipmentTable_->item( row, 1 )->setText( "0" );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::FillEquipments
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::FillEquipments( actions::parameters::ParameterList& list )
{
    int index = 0;
    for( int nRow = 0; nRow < equipmentTable_->rowCount(); ++nRow )
    {
        int quantity = equipmentTable_->item( nRow, 1 )->text().toInt();
        if( quantity == 0 )
            continue;
        unsigned int id = equipmentIdMap_[ equipmentTable_->item( nRow, 0 )->text() ];
        actions::parameters::ParameterList& personalList = list.AddList( QString( "Equipment %L1" ).arg( index++ ).toStdString() );
        personalList.AddIdentifier( "Equipment", id );
        personalList.AddQuantity( "Quantity", quantity );
    }
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::CheckValidity
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
bool EquipmentTransferDialog::CheckValidity() const
{
    for( int nRow = 0; nRow < equipmentTable_->rowCount(); ++nRow )
        if( equipmentTable_->item( nRow, 1 )->text().toInt() != 0 )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: EquipmentTransferDialog::Validate
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void EquipmentTransferDialog::Validate()
{
    if( !selectedFrom_ )
        return;
    if( !selectedTo_ || !CheckValidity() )
    {
        QMessageBox::warning( this, tr( "SWORD" ), ( !selectedTo_ ) ? tr( "Invalid destination unit." ) : tr( "Invalid equipment quantity, at least one must be > 0." ) );
        return;
    }
    accept();
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "transfer_equipment" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new actions::parameters::Identifier( it.NextElement(), selectedTo_->GetId() ) );
    actions::parameters::ParameterList* equipments = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *equipments );
    FillEquipments( *equipments );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selectedFrom_, false ) );
    actionsModel_.Publish( *action, 0 );
    selectedFrom_ = 0;
    selectedTo_ = 0;
}
