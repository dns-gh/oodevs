// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyChangeQuotasDialog.h"
#include "moc_LogisticSupplyChangeQuotasDialog.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "gaming/AgentsModel.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticLinks.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "tools/Iterator.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "protocol/simulationsenders.h"

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const Model& model, const Profile_ABC& profile )
    : QDialog( parent, tr( "Supply quotas allocation" ) )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , model_( model )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Supply quotas allocation" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Target:" ), box );
    targetCombo_ = new ValuedComboBox< const Automat_ABC* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 2, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Dotation" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quota" ) );
    table_->setLeftMargin( 0 );
    table_->setMinimumSize( 220, 200 );
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
// Name: LogisticSupplyChangeQuotasDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::~LogisticSupplyChangeQuotasDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        const AutomatType& type = agent.GetType();
        if( type.IsLogisticSupply() )
        {
            selected_ = &agent;
            menu.InsertItem( "Command", tr( "Allocate supply quotas" ), this, SLOT( Show() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Show()
{
    if( !selected_ )
        return;

    targetCombo_->Clear();
    tools::Iterator< const Automat_ABC& > it = model_.agents_.Resolver< Automat_ABC >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& agent = it.NextElement();
        const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
        if( log && ( log->GetSupply() == selected_ || ( log->GetSupply() == 0 && log->GetTC2() == selected_ ) ) )
            targetCombo_->AddItem( agent.GetName(), &agent );
    }
    OnSelectionChanged();
    show();
}

namespace
{
    std::string CreateName( const std::string& str, unsigned int index )
    {
        return str + " " + boost::lexical_cast< std::string >( index );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Validate()
{
    const Automat_ABC* target = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !target )
        return;

    targetCombo_->setFocus();

    accept();

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "log_supply_change_quotas" );
    UnitMagicAction* action = new UnitMagicAction( *target, actionType, controllers_.controller_, tr( "Log Supply Change Quotas" ), true );

    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Automat( it.NextElement(), *selected_, controllers_.controller_ ) );

    parameters::ParameterList* dotations = new parameters::ParameterList( it.NextElement() );

    action->AddParameter( *dotations );
    unsigned int index = 0;
    for( int i = 0; i < table_->numRows(); ++i )
    {
        const QString text = table_->text( i, 0 );
        if( text.isEmpty() || text.isNull() )
            continue;
        ParameterList& dotationList = dotations->AddList( CreateName( "Dotation", ++index ) );
        dotationList.AddIdentifier( "Type", supplies_[ text ].type_->GetId() );
        dotationList.AddQuantity( "Number", table_->text( i, 1 ).toInt() );
    }
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( target, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnSelectionChanged()
{
    supplies_.clear();
    dotationTypes_.clear();
    dotationTypes_.append( "" );
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( agent )
    {
        // $$$$ AGE 2006-10-06: use LogisticHierarchies ?
        const CommunicationHierarchies& hierarchies = agent->Get< CommunicationHierarchies >();
        tools::Iterator< const Entity_ABC& > children = hierarchies.CreateSubordinateIterator();
        while( children.HasMoreElements() )
            AddDotation( children.NextElement() );
    }
    table_->setNumRows( 0 );
    if( ! dotationTypes_.empty() )
        AddItem();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::AddDotation
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddDotation( const kernel::Entity_ABC& entity )
{
    if( const SupplyStates* states = entity.Retrieve< SupplyStates >() )
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
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnValueChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnValueChanged( int row, int col )
{
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;

    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( col == 0 )
    {
        if( item.currentItem() == 0 && row != table_->numRows() - 1 )
            table_->removeRow( row );
        else if( item.currentItem() && row == table_->numRows() - 1 )
        {
            const int current = item.currentItem();
            if( table_->numRows() < int( dotationTypes_.size() ) - 1 )
                AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
    }
    else if( col == 1 )
    {
        // $$$$ SBO 2006-07-03: check value/stock
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::AddItem
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddItem()
{
    const Automat_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
