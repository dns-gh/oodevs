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

#include "actions/ActionsModel.h"
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
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
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
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* parent,
                                                                    Controllers& controllers,
                                                                    actions::ActionsModel& actionsModel,
                                                                    const ::StaticModel& staticModel,
                                                                    const kernel::Time_ABC& simulation,
                                                                    const Model& model )
    : QDialog( parent, tr( "Supply quotas allocation" ) )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , model_( model )
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

    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::~LogisticSupplyChangeQuotasDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Show( const kernel::Entity_ABC& entity )
{
    selected_ = &entity;
    targetCombo_->Clear();
    {
        tools::Iterator< const Automat_ABC& > it = model_.agents_.Resolver< Automat_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& agent = it.NextElement();
            const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
            if( log && log->HasSuperior( *selected_ ) && agent.Get< gui::LogisticBase >().IsBase() )
                targetCombo_->AddItem( agent.GetName(), &agent );
        }
    }
    {
        tools::Iterator< const Formation_ABC& > it = model_.teams_.Resolver< Formation_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& agent = it.NextElement();
            const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
            if( log && log->HasSuperior( *selected_ ) && agent.Get< gui::LogisticBase >().IsBase() )
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
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "log_supply_change_quotas" );
    std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), selected_->GetId() ) );

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
    action->Attach( *new ActionTasker( controllers_.controller_, target, false ) );
    actionsModel_.Publish( *action );
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
