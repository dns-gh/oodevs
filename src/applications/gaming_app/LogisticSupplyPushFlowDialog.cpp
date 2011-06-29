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
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const tools::Resolver_ABC< Automat_ABC >& automats, const Profile_ABC& profile )
    : QDialog( parent, tr( "Push supply flow" ) )
    , controllers_( controllers )
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
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Formation_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::InsertMenuEntry
// Created: ABR 2011-06-29
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::InsertMenuEntry( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu )
{
    selected_ = &agent;
    menu.InsertItem( "Command", tr( "Push supply flow" ), this, SLOT( Show() ) );
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
        if( (const Entity_ABC*)&automat != selected_ )
        {
            const kernel::AutomatType& type = automat.GetType();
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
    targetCombo_->setFocus();
    accept();

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( dynamic_cast< const Automat_ABC* >( static_cast< const Entity_ABC* >( selected_ ) ) ) ? "automat_log_supply_push_flow" : "formation_log_supply_push_flow" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Log Supply Push Flow" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

    action->AddParameter( *new parameters::Automat( it.NextElement(), *target, controllers_.controller_ ) );
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
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
    //// $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    //MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( selectedAutomat_ ) ? "automat_log_supply_push_flow" : "formation_log_supply_push_flow" );
    //UnitMagicAction* action = new UnitMagicAction( *target, actionType, controllers_.controller_, tr( "Log Supply Push Flow" ), true );
    //tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    //if( selectedAutomat_ )
    //{
    //    assert( dynamic_cast<const Automat_ABC*>( (const Entity_ABC*)selected_) );
    //    action->AddParameter( *new parameters::Automat( it.NextElement(), *dynamic_cast< const Automat_ABC* >( ( const Entity_ABC* )selected_), controllers_.controller_ ) );
    //}
    //else
    //{
    //    assert( dynamic_cast<const Formation_ABC*>( (const Entity_ABC*)selected_) );
    //    action->AddParameter( *new parameters::Formation( it.NextElement(), *dynamic_cast< const Formation_ABC* >( ( const Entity_ABC* )selected_), controllers_.controller_ ) );
    //}

    //parameters::ParameterList* dotations = new parameters::ParameterList( it.NextElement() );
    //action->AddParameter( *dotations );

    //unsigned int rows = 0;
    //for( int i = 0; i < table_->numRows(); ++i )
    //    if( !table_->item( i, 0 )->text().isEmpty() )
    //        ++rows;

    //if( rows > 0 )
    //{
    //    int index = 1;
    //    for( int i = 0; i < table_->numRows(); ++i )
    //    {
    //        const QString text = table_->text( i, 0 );
    //        if( text.isEmpty() )
    //            continue;
    //        ParameterList& dotationList = dotations->AddList( CreateName( "Dotation", index ) );
    //        dotationList.AddIdentifier( "Type", supplies_[ text ].type_->GetId() );
    //        dotationList.AddQuantity( "Number", table_->text( i, 1 ).toInt() );
    //    }
    //}
    //action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    //action->Attach( *new ActionTasker( target, false ) );
    //action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    reject();
    selected_ = 0;
}

namespace
{

    struct SupplyStatesVisitor : private boost::noncopyable
                               , public kernel::ExtensionVisitor_ABC<SupplyStates>
    {
        SupplyStatesVisitor( LogisticSupplyPushFlowDialog& dlg, void (LogisticSupplyPushFlowDialog::*pFunc)(const SupplyStates&) )
                : dlg_(dlg), pFunc_ ( pFunc ) {}

        void Visit( const SupplyStates& extension )
        {
            (dlg_.*pFunc_)(extension);
        }
    private:
        LogisticSupplyPushFlowDialog& dlg_;
        void (LogisticSupplyPushFlowDialog::*pFunc_)(const SupplyStates&);

    };
}

// -----------------------------------------------------------------------------
// Name: formation::OnSelectionChanged
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnSelectionChanged()
{
    supplies_.clear();
    dotationTypes_.clear();
    dotationTypes_.append( "" );
    if( selected_ )
    {
        SupplyStatesVisitor visitor( *this, &LogisticSupplyPushFlowDialog::AddDotation );
        selected_->Get< kernel::TacticalHierarchies >().Accept<SupplyStates>(visitor);
    }
    table_->setNumRows( 0 );
    if( ! dotationTypes_.empty() )
        AddItem();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddDotation
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddDotation( const SupplyStates& states )
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
