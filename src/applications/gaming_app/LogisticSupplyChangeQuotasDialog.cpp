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
#include "LogisticSupplyValuesTableWidget.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "actions/Identifier.h"
#include "gaming/AgentsModel.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/LogisticLinks.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "gaming/TeamsModel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/SimulationSenders.h"
#include <tools/Iterator.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder constructor
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::SelectedHolder::SelectedHolder( kernel::Controllers& controllers )
    : selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::Selected
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* LogisticSupplyChangeQuotasDialog::SelectedHolder::Selected() const
{
    const kernel::Entity_ABC* retval = 0;
    if( selectedAutomat_ )
        retval = ( const kernel::Entity_ABC* )selectedAutomat_;
    if( selectedFormation_ )
        retval = ( const kernel::Entity_ABC* )selectedFormation_;
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::Set
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::SelectedHolder::Set( const kernel::Automat_ABC& agent )
{
    selectedAutomat_ = &agent;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::Set
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::SelectedHolder::Set( const kernel::Formation_ABC& agent )
{
    selectedFormation_ = &agent;
    selectedAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::operator const kernel::Entity_ABC*
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::SelectedHolder::operator const kernel::Entity_ABC* () const
{
    return Selected();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::Reset
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::SelectedHolder::Reset()
{
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::GetParameter
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
actions::Parameter_ABC* LogisticSupplyChangeQuotasDialog::SelectedHolder::GetParameter( const kernel::OrderParameter& parameter, kernel::Controller& controller )
{
    actions::Parameter_ABC* retval = 0;
    if( selectedAutomat_ )
        retval =  new parameters::Automat( parameter, *selectedAutomat_, controller );
    else if( selectedFormation_ )
        retval =  new parameters::Formation( parameter, *selectedFormation_, controller );
    return retval;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SelectedHolder::GetMagicActionType
// Created: ABR 2011-04-11
// -----------------------------------------------------------------------------
const std::string LogisticSupplyChangeQuotasDialog::SelectedHolder::GetMagicActionType() const
{
    if( selectedAutomat_ != 0)
        return "automat_log_supply_change_quotas";
    else if( selectedFormation_ )
        return "formation_log_supply_change_quotas";
    else
        throw MASA_EXCEPTION( "SelectedHolder: nor automat nor formation set." );
}

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
    setFixedSize( 375, 265 );

    QGridLayout* layout = new QGridLayout( this, 3, 2, 5 );
    layout->setRowMinimumHeight( 0, 25 );
    layout->setColumnMinimumWidth( 0, 110 );
    targetCombo_ = new ValuedComboBox< const Entity_ABC* >( "targetCombo", this );
    targetCombo_->setMinimumWidth( 150 );

    QStringList header;
    header << tr( "Resource" ) << tr( "Quota" );
    table_ = new LogisticSupplyValuesTableWidget( this, header );
    table_->setMinimumSize( 220, 200 );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), this );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), this );
    okButton->setDefault( true );

    layout->addWidget( new QLabel( tr( "Target:" ), this ), 0, 0, 1, 1, Qt::AlignLeft );
    layout->addWidget( targetCombo_, 0, 1, 1, 1 );
    layout->addWidget( table_, 1, 0, 1, 2 );
    layout->addWidget( okButton, 2, 0, 1, 1 );
    layout->addWidget( cancelButton, 2, 1, 1, 1 );

    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( targetCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSelectionChanged() ) );

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
        if( agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        {
            selected_.Set( agent );
            menu.InsertItem( "Command", tr( "Allocate supply quotas" ), this, SLOT( Show() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::NotifyContextMenu( const kernel::Formation_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        if( agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        {
            selected_.Set( agent );
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
    {
        tools::Iterator< const Automat_ABC& > it = model_.agents_.Resolver< Automat_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& agent = it.NextElement();
            const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
            if( log && log->HasSuperior( *selected_ ) && agent.GetLogisticLevel() != LogisticLevel::none_ )
                targetCombo_->AddItem( agent.GetName(), &agent );
        }
    }
    {
        tools::Iterator< const Formation_ABC& > it = model_.teams_.Resolver< Formation_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& agent = it.NextElement();
            const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
            if( log && log->HasSuperior( *selected_ ) && agent.GetLogisticLevel() != LogisticLevel::none_ )
                targetCombo_->AddItem( agent.GetName(), &agent );
        }
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
    const Entity_ABC* target = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( !selected_ || !target )
        return;
    targetCombo_->setFocus();
    accept();

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( selected_.GetMagicActionType() );
    UnitMagicAction* action = new UnitMagicAction( *target, actionType, controllers_.controller_, tr( "Log Supply Change Quotas" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), (*selected_).GetId() ) );

    parameters::ParameterList* dotations = new parameters::ParameterList( it.NextElement() );
    action->AddParameter( *dotations );

    unsigned int index = 0;
    QMap< QString, int > quantities;
    table_->GetQuantities( quantities );
    for( QMap< QString, int >::iterator it = quantities.begin(); it != quantities.end(); ++it )
    {
        ParameterList& dotationList = dotations->AddList( CreateName( "Dotation", ++index ) );
        dotationList.AddIdentifier( "Type", supplies_[ it.key() ].type_->GetId() );
        dotationList.AddQuantity( "Number", it.value() );
    }
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
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
    selected_.Reset();
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
    const Entity_ABC* agent = targetCombo_->count() ? targetCombo_->GetValue() : 0;
    if( agent )
        logistic_helpers::VisitBaseStocksDotations( *agent, boost::bind( &LogisticSupplyChangeQuotasDialog::AddDotation, this, _1 ) );
    QMap< QString, int > values;
    QStringList possible = supplies_.keys();
    table_->SetQuantities( values, possible );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::AddDotation
// Created: MMC 2012-10-22
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::AddDotation( const Dotation& dotation )
{
    if( !dotation.type_ )
        return;
    const QString type = dotation.type_->GetName().c_str();
    Dotation& supply = supplies_[ type ];
    if( !supply.type_ )
    {
        dotationTypes_.append( type );
        supply.type_ = dotation.type_;
    }
}
