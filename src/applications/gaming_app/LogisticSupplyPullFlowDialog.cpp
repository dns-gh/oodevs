// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyPullFlowDialog.h"
#include "moc_LogisticSupplyPullFlowDialog.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "gaming/Dotation.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "protocol/SimulationSenders.h"
#include <boost/noncopyable.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog constructor
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
LogisticSupplyPullFlowDialog::LogisticSupplyPullFlowDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel,
        const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const tools::Resolver_ABC< Automat_ABC >& automats,
        const tools::Resolver_ABC< kernel::Formation_ABC >& formations, const Profile_ABC& profile )
    : QDialog( parent, tr( "Pull supply flow" ) )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , automats_( automats )
    , formations_( formations )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Pull supply flow" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );

    QHBox* box = new QHBox( this );
    box->setMargin( 5 );
    new QLabel( tr( "Target:" ), box );
    targetCombo_ = new ValuedComboBox< const Entity_ABC* >( box );
    targetCombo_->setMinimumWidth( 150 );
    layout->addWidget( box );

    table_ = new QTable( 0, 3, this );
    table_->setMargin( 5 );
    table_->horizontalHeader()->setLabel( 0, tr( "Resource" ) );
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
// Name: LogisticSupplyPullFlowDialog destructor
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
LogisticSupplyPullFlowDialog::~LogisticSupplyPullFlowDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::NotifyContextMenu
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        if( agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        {
            selected_ = &agent;
            menu.InsertItem( "Command", tr( "Pull supply flow" ), this, SLOT( Show() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Show
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Show()
{
    if( !selected_ )
        return;

    targetCombo_->Clear();
    const kernel::Entity_ABC& team = selected_->Get< kernel::TacticalHierarchies >().GetTop();
    {
        tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& automat = it.NextElement();
            if( &automat != selected_ )
            {
                if( automat.GetLogisticLevel()!=kernel::LogisticLevel::none_ && &automat.Get< kernel::TacticalHierarchies >().GetTop() == &team )
                    targetCombo_->AddItem( automat.GetName(), &automat );
            }
        }
    }
    {
        tools::Iterator< const Formation_ABC& > it = formations_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& formation = it.NextElement();
            if( formation.GetLogisticLevel()!=kernel::LogisticLevel::none_ && &formation.Get< kernel::TacticalHierarchies >().GetTop() == &team )
                targetCombo_->AddItem( formation.GetName(), &formation );
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
// Name: LogisticSupplyPullFlowDialog::Validate
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Validate()
{
    const Entity_ABC* target = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !target )
        return;
    targetCombo_->setFocus();
    accept();

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    bool IsAutomat = dynamic_cast< const Automat_ABC* >( target ) != 0;
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( IsAutomat ) ? "automat_log_supply_pull_flow" : "formation_log_supply_pull_flow" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Log Supply Pull Flow" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( ( IsAutomat )
        ? ( Parameter_ABC& ) *new parameters::Automat( it.NextElement(), *dynamic_cast< const Automat_ABC* >( target ), controllers_.controller_ )
        : ( Parameter_ABC& ) *new parameters::Formation( it.NextElement(), *dynamic_cast< const Formation_ABC* >( target ), controllers_.controller_ ) );

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
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Reject
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Reject()
{
    reject();
    selected_ = 0;
}

namespace
{

    struct SupplyStatesVisitor : kernel::ExtensionVisitor_ABC<SupplyStates>, boost::noncopyable
    {
        SupplyStatesVisitor( LogisticSupplyPullFlowDialog& dlg, void (LogisticSupplyPullFlowDialog::*pFunc)(const SupplyStates&) )
                : dlg_(dlg), pFunc_ ( pFunc ) {}

        void Visit( const SupplyStates& extension )
        {
            (dlg_.*pFunc_)(extension);
        }
    private:
        LogisticSupplyPullFlowDialog& dlg_;
        void (LogisticSupplyPullFlowDialog::*pFunc_)(const SupplyStates&);

    };
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::OnSelectionChanged
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnSelectionChanged()
{
    supplies_.clear();
    dotationTypes_.clear();
    dotationTypes_.append( "" );
    const Entity_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( agent )
    {
        const TacticalHierarchies& hierarchies = agent->Get< TacticalHierarchies >();
        SupplyStatesVisitor visitor( *this, &LogisticSupplyPullFlowDialog::AddDotation );
        hierarchies.Accept<SupplyStates>(visitor);
    }
    table_->setNumRows( 0 );
    if( ! dotationTypes_.empty() )
        AddItem();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddDotation
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddDotation( const SupplyStates& states )
{
    tools::Iterator< const Dotation& > it = states.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        const QString type = dotation.type_->GetName().c_str();
        Dotation& supply = supplies_[ type ];
        if( ! supply.type_ )
        {
            dotationTypes_.append( type );
            supply.type_ = dotation.type_;
        }
        supply.quantity_ += dotation.quantity_;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::OnValueChanged
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnValueChanged( int row, int col )
{
    const Entity_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
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
// Name: LogisticSupplyPullFlowDialog::AddItem
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddItem()
{
    const Entity_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !agent )
        return;
    const unsigned int rows = table_->numRows() + 1;
    table_->setNumRows( rows );
    table_->setItem( rows - 1, 0, new ExclusiveComboTableItem( table_, dotationTypes_ ) );
    table_->setCurrentCell( rows - 1, 1 );
}
