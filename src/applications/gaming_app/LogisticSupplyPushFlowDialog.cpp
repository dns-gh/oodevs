// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyPushFlowDialog.h"
#include "moc_LogisticSupplyPushFlowDialog.cpp"
#include "actions/UnitMagicAction.h"
#include "actions/ParameterList.h"
#include "actions/Automat.h"
#include "gaming/ActionPublisher.h"
#include "gaming/ActionTiming.h"
#include "gaming/Dotation.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "protocol/simulationsenders.h"

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, const tools::Resolver_ABC< Automat_ABC >& automats, const Profile_ABC& profile )
    : QDialog( parent, tr( "Push supply flow" ) )
    , controllers_( controllers )
    , actionPublisher_( actionPublisher )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , automats_( automats )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Push supply flow" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Target:" ), box );
    targetCombo_ = new ValuedComboBox< const Automat_ABC* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 3, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Dotation" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quantity" ) );
    table_->horizontalHeader()->setLabel( 2, tr( "Available" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 300, 200 );
    table_->setColumnReadOnly( 2, true );
    layout->addWidget( table_ );

    box = new QHBox( this );
    box->setMargin( 5 );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), box );
    okButton->setDefault( true );
    layout->addWidget( box );

    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );

    connect( targetCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSelectionChanged() ) );
    connect( table_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::~LogisticSupplyPushFlowDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        const AutomatType& type = agent.GetType();
        if( type.IsLogisticSupply() )
        {
            selected_ = &agent;
            menu.InsertItem( "Command", tr( "Push supply flow" ), this, SLOT( Show() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Show()
{
    if( !selected_ )
        return;

    targetCombo_->Clear();
    tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
    const kernel::Entity_ABC& team = selected_->Get< kernel::TacticalHierarchies >().GetTop();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        if( &automat != selected_ )
        {
            const AutomatType& type = automat.GetType();
            if( type.IsLogisticSupply() && &automat.Get< kernel::TacticalHierarchies >().GetTop() == &team )
                targetCombo_->AddItem( automat.GetName(), &automat );
        }
    }
    OnSelectionChanged();
    show();
}

namespace
{
    std::string CreateName( const std::string& str, int& index )
    {
        return QString( (str + " %1" ).c_str() ).arg( index++ ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Validate()
{
    const Automat_ABC* target = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !target )
        return;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "log_supply_push_flow" );
    UnitMagicAction* action = new UnitMagicAction( *target, actionType, controllers_.controller_, true );
    
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), *selected_, controllers_.controller_ ) );

    parameters::ParameterList* dotations = new parameters::ParameterList( it.NextElement() );
    
    action->AddParameter( *dotations );

    unsigned int rows = 0;
    for( int i = 0; i < table_->numRows(); ++i )
        if( !table_->item( i, 0 )->text().isEmpty() )
            ++rows;

    if( rows > 0 )
    {
        int index = 1;
        for( int i = 0; i < table_->numRows(); ++i )
        {
            const QString text = table_->text( i, 0 );
            if( text.isEmpty() )
                continue;

            ParameterList& dotationList = dotations->AddList( CreateName( "Dotation", index ) );
            dotationList.AddIdentifier( "Type", supplies_[ text ].type_->GetId() );
            dotationList.AddQuantity( "Number", table_->text( i, 1 ).toInt() );
        }
    }

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_, actionPublisher_ );

    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnSelectionChanged()
{
    supplies_.clear();
    dotationTypes_.clear();
    dotationTypes_.append( "" );
    if( selected_ )
        AddDotation( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddDotation
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddDotation( const kernel::Entity_ABC& entity )
{
    const SupplyStates* states = entity.Retrieve< SupplyStates >();
    if( states )
    {
        tools::Iterator< const Dotation& > it = states->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& dotation = it.NextElement();
            const QString type = dotation.type_->GetCategory().c_str();
            Dotation& supply = supplies_[ type ];
            if( ! supply.type_ )
            {
                dotationTypes_.append( type );
                supply.type_ = dotation.type_;
            }
            supply.quantity_ += dotation.quantity_;
        }
        table_->setNumRows( 0 );
        AddItem();
    }
    tools::Iterator< const Entity_ABC& > it( entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator() );
    while( it.HasMoreElements() )
        AddDotation( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnValueChanged( int row, int col )
{
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;

    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( col == 0 )
    {
        if( item.currentItem() == 0 && row != table_->numRows() - 1 )
        {
            table_->removeRow( row );
            table_->setCurrentCell( table_->numRows() - 1, 1 );
            return;
        }
        if( item.currentItem() && row == table_->numRows() - 1 )
        {
            const int current = item.currentItem();
            if( table_->numRows() < int( dotationTypes_.size() ) - 1 )
                AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
        if( ! table_->text( row, 0 ).isEmpty() )
        {
            table_->setText( row, 1, "1" );
            table_->setText( row, 2, QString::number( supplies_[ table_->text( row, 0 ) ].quantity_ ) );
            table_->adjustColumn( 0 );
            table_->adjustColumn( 1 );
            table_->adjustColumn( 2 );
        }
        else
        {
            table_->setText( row, 1, "" );
            table_->setText( row, 2, "" );
        }
    }
    else if( col == 1 )
    {
        // $$$$ SBO 2006-07-03: check value/stock 
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddItem
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddItem()
{
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
