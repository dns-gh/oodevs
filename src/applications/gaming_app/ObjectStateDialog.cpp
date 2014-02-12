// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectStateDialog.h"
#include "moc_ObjectStateDialog.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/StaticModel.h"
#include "gaming/MineAttribute.h"
#include "gaming/ConstructionAttribute.h"
#include "gaming/ObstacleAttribute.h"
#include "gaming/BypassAttribute.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog constructor
// Created: NPT 2012-09-18
// -----------------------------------------------------------------------------
ObjectStateDialog::ObjectStateDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent )
    , controllers_( controllers )
    , static_( staticModel )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Change object state" ) );
    setModal( true );

    QLabel* constructionLabel = new QLabel( tr( "Construction:" ) );
    constructionSpinBox_ = new QSpinBox();
    constructionSpinBox_->setRange( 0, 100 );
    constructionSpinBox_->setSuffix( "%" );
    constructionSpinBox_->setEnabled( false );

    QLabel* miningLabel = new QLabel( tr( "Mining:" ) );
    miningSpinBox_ = new QSpinBox();
    miningSpinBox_->setRange( 0, 100 );
    miningSpinBox_->setSuffix( "%" );
    miningSpinBox_->setEnabled( false );

    QLabel* bypassLabel = new QLabel( tr( "Bypass:" ) );
    bypassSpinBox_ = new QSpinBox();
    bypassSpinBox_->setRange( 0, 100 );
    bypassSpinBox_->setSuffix( "%" );
    bypassSpinBox_->setEnabled( false );

    QWidget* propertiesPanel = new QWidget( this );
    QGridLayout* propertiesLayout = new QGridLayout( propertiesPanel );
    propertiesLayout->addWidget( constructionLabel, 0, 0 );
    propertiesLayout->addWidget( constructionSpinBox_, 0, 1 );
    propertiesLayout->addWidget( miningLabel, 1, 0 );
    propertiesLayout->addWidget( miningSpinBox_, 1, 1 );
    propertiesLayout->addWidget( bypassLabel, 2, 0 );
    propertiesLayout->addWidget( bypassSpinBox_, 2, 1 );

    QPushButton* applyButton = new QPushButton( tr( "Ok" ) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    applyButton->setDefault( true );
    connect( applyButton, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( OnCancel() ) );

    QWidget* buttonPanel = new QWidget( this );
    QHBoxLayout* pButtonLayout = new QHBoxLayout( buttonPanel );
    pButtonLayout->addWidget( applyButton, Qt::AlignCenter );
    pButtonLayout->addWidget( cancelButton, Qt::AlignCenter );
    pButtonLayout->setMargin( 10 );

    QVBoxLayout* generalLayout = new QVBoxLayout( this );
    generalLayout->addWidget( propertiesPanel );
    generalLayout->addWidget( buttonPanel );

    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog destructor
// Created: NPT 2012-09-18
// -----------------------------------------------------------------------------
ObjectStateDialog::~ObjectStateDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog::OnOk
// Created: NPT 2012-09-18
// -----------------------------------------------------------------------------
void ObjectStateDialog::OnOk()
{
    if( selected_ )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "update_object" );
        std::unique_ptr< actions::Action_ABC > action( new actions::ObjectMagicAction( actionType, controllers_.controller_, false ) );
        action->Rename( tools::translate( "gaming_app::Action", "Object Update" ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

        actions::parameters::ParameterList* attributesList = new actions::parameters::ParameterList( it.NextElement() );
        action->AddParameter( *attributesList );

        if( selected_->Retrieve< kernel::ConstructionAttribute_ABC >() )
        {
            actions::parameters::ParameterList& constructionList = attributesList->AddList( "Construction" );
            constructionList.AddIdentifier( "AttributeId", sword::ObjectMagicAction::construction );
            constructionList.AddIdentifier( "Type", 0 );
            constructionList.AddQuantity( "Number", 0 );
            constructionList.AddNumeric( "Density", 0 );
            constructionList.AddQuantity( "Percentage", constructionSpinBox_->value() );
        }

        if( selected_->Retrieve< kernel::MineAttribute_ABC >() )
        {
            actions::parameters::ParameterList& mineList = attributesList->AddList( "Mine" );
            mineList.AddIdentifier( "AttributeId", sword::ObjectMagicAction::mine );
            mineList.AddIdentifier( "Type", 0 );
            mineList.AddQuantity( "Number", 0 );
            mineList.AddNumeric( "Density", 0 );
            mineList.AddQuantity( "Percentage", miningSpinBox_->value() );
        }
        if( selected_->Retrieve< kernel::BypassAttribute_ABC >() )
        {
            actions::parameters::ParameterList& bypassList = attributesList->AddList( "Bypass" );
            bypassList.AddIdentifier( "AttributeId", sword::ObjectMagicAction::bypass );
            bypassList.AddQuantity( "Percentage", bypassSpinBox_->value() );
        }
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        actionsModel_.Publish( *action );
    }
    selected_ = 0;
    accept();
}
// -----------------------------------------------------------------------------
// Name: ObjectStateDialog::OnCancel
// Created: NPT 2012-09-18
// -----------------------------------------------------------------------------
void ObjectStateDialog::OnCancel()
{
    selected_ = 0;
    reject();
}

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog::Show
// Created: NPT 2012-09-19
// -----------------------------------------------------------------------------
void ObjectStateDialog::Show()
{
    UpdateDialog();
    exec();
}

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog::NotifyContextMenu
// Created: NPT 2012-09-18
// -----------------------------------------------------------------------------
void ObjectStateDialog::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    if( (object.Retrieve< kernel::MineAttribute_ABC >()
     || object.Retrieve< kernel::ConstructionAttribute_ABC>()
     || object.Retrieve< kernel::BypassAttribute_ABC >() )
     && controllers_.GetCurrentMode() != eModes_Replay )
    {
        selected_ = &object;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
        subMenu->insertItem( tr( "Change object state" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectStateDialog::UpdateDialog
// Created: NPT 2012-09-19
// -----------------------------------------------------------------------------
void ObjectStateDialog::UpdateDialog()
{
    if( !selected_ )
        return;
    miningSpinBox_->setEnabled( false );
    constructionSpinBox_->setEnabled( false );
    bypassSpinBox_->setEnabled( false );
    miningSpinBox_->setValue( 0 );
    constructionSpinBox_->setValue( 0 );
    bypassSpinBox_->setValue( 0 );
    if( const MineAttribute* mine = static_cast< const MineAttribute* >( selected_->Retrieve< kernel::MineAttribute_ABC >() ) )
    {
        miningSpinBox_->setValue( mine->GetValorizationPercentage() );
        miningSpinBox_->setEnabled( true );
    }
    if( const ConstructionAttribute* construction = static_cast< const ConstructionAttribute* >( selected_->Retrieve< kernel::ConstructionAttribute_ABC >() ) )
    {
        constructionSpinBox_->setValue( construction->GetConstructionPercentage() );
        constructionSpinBox_->setEnabled( true );
    }
    if( const BypassAttribute* bypass = static_cast< const BypassAttribute* >( selected_->Retrieve< kernel::BypassAttribute_ABC >() ) )
    {
        bypassSpinBox_->setValue( bypass->GetBypassConstructionPercentage() );
        bypassSpinBox_->setEnabled( true );
    }
}
