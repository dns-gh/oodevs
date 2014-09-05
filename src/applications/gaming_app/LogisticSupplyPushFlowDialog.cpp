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
#include "LogisticRouteWidget.h"
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "LogisticSupplyCarriersTableWidget.h"
#include "LogisticSupplyExclusiveListWidget.h"

#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ActionsModel.h"
#include "actions/PushFlowParameters.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotations.h"
#include "gaming/LogisticLinks.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/StaticModel.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent,
                                                            kernel::Controllers& controllers,
                                                            actions::ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel,
                                                            const kernel::Time_ABC& simulation,
                                                            const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                                            const kernel::Profile_ABC& profile,
                                                            const gui::EntitySymbols& symbols )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, automats, profile, symbols )
    , isPushFlow_( true )
    , lastContext_( 0 )
{
    recipientsList_ = new LogisticSupplyExclusiveListWidget( this, tr( "Add recipient" ), tr( "Remove recipient" ) );
    connect( recipientsList_, SIGNAL( ItemAdded( const QString& ) ), SLOT( AddRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( ItemRemoved( const QString& ) ), SLOT( RemoveRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( SelectionChanged( const QString&, const QString& ) ),
                              SLOT( OnRecipientSelectionChanged( const QString&, const QString& ) ) );
    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab_ );
    resourcesLayout->addWidget( recipientsList_ );
    resourcesLayout->addWidget( resourcesTable_ );
    actionsModel.RegisterHandler( [&]( const sword::SimToClient& message )
    {
        if( !lastContext_ || !isVisible() ||
            !message.message().has_unit_magic_action_ack() || lastContext_ != message.context() )
            return;
        if( message.message().unit_magic_action_ack().error_code() != sword::UnitActionAck_ErrorCode_no_error )
        {
            QMessageBox::warning( this, tr( "SWORD" ), GetErrorText( message.message().unit_magic_action_ack() ) );
            EnableButtons( true );
            return;
        }
        accept();
        Clear();
    } );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::~LogisticSupplyPushFlowDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::PushFlow
// Created: JSR 2013-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::PushFlow( const kernel::Entity_ABC& entity )
{
    selected_ = &entity;
    isPushFlow_ = true;
    setCaption( tr( "Push supply flow" ) );
    Show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Supply
// Created: JSR 2013-04-15
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Supply( const kernel::Entity_ABC& entity )
{
    selected_ = &entity;
    isPushFlow_ = false;
    setCaption( tr( "Resupply" ) );
    Show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Show
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Show()
{

    logistic_helpers::VisitPartialBaseStocksDotations( *selected_,
                                                       [&]( const Dotation& dotation ){ AddAvailable( dotation ); },
                                                       [&]( const kernel::Entity_ABC& entity ){ return profile_.CanBeOrdered( entity ); } );

    ComputeRecipients();
    tabs_->setCurrentPage( 0 );

    QStringList recipientsList;
    ComputeAvailableRecipients( recipientsList );
    recipientsList_->SetChoice( recipientsList );

    QMap< QString, int > carriersQty, carriersAvailable;
    carriersUseCheck_->setCheckState( Qt::Unchecked );
    ComputeAvailableCarriers( carriersAvailable );
    carriersTable_->SetQuantities( carriersQty, carriersAvailable );
    carriersTable_->setEnabled( false );

    route_->AddRequester( selected_ );
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Validate()
{
    if( carriersUseCheck_->isChecked() && profile_.IsSupervision() )
    {
        if( carriersTable_->IsIncomplete() )
        {
            QMessageBox::critical( this, tr( "Error" ), tr( "The convoy is unable to carry all types of supplies" ) );
            return;
        }
        if( carriersTable_->IsOverloaded() )
        {
            QMessageBox::critical( this, tr( "Error" ), tr( "The convoy is unable to carry that much weight and/or volume" ) );
            return;
        }
        if( carriersTable_->IsUnderloaded() &&
            QMessageBox::warning( this, tr( "Error" ),
                tr( "The convoy is under its minimal mass and/or volume threshold. Do you want to continue?" ),
                QMessageBox::Ok, QMessageBox::Cancel | QMessageBox::Escape ) != QMessageBox::Ok )
            return;
    }

    if( pRecipientSelected_ )
        GetSuppliesFromTable( *pRecipientSelected_ );
    T_QuantitiesMap carriers;
    if( carriersUseCheck_->isChecked() )
        carriersTable_->GetQuantities( carriers );

    if( !selected_ )
        return Reject();

    EnableButtons( false );

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "log_supply_push_flow" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

    actions::parameters::PushFlowParameters* pushFlowParameters = new actions::parameters::PushFlowParameters( it.NextElement(), static_.coordinateConverter_, !isPushFlow_ );
    for( auto it = recipients_.begin(); it != recipients_.end(); ++it )
    {
        const T_QuantitiesMap& supplyQuantities = recipientSupplies_[ *it ];
        for( auto itResource = supplyQuantities.begin(); itResource != supplyQuantities.end(); ++itResource )
        {
            const kernel::DotationType* dotationType = availableSupplies_[ itResource.key() ].type_;
            assert( dotationType );
            if( dotationType )
                pushFlowParameters->AddResource( *dotationType, itResource.value(), **it );
        }
    }
    for( auto it = carriers.begin(); it != carriers.end(); ++it )
        pushFlowParameters->AddTransporter( *carriersTypeNames_[ it.key() ], it.value() );

    route_->FillPushFlowParameters( *pushFlowParameters );

    action->AddParameter( *pushFlowParameters );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    lastContext_ = actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    reject();
    Clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddRecipient
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddRecipient( const QString& recipientName )
{
    const kernel::Automat_ABC* pRecipient = recipientsNames_.value( recipientName );
    if( !pRecipient )
        return;
    if( recipientSupplies_.find( pRecipient ) == recipientSupplies_.end() )
        recipientSupplies_[ pRecipient ];
    if( std::find( recipients_.begin(), recipients_.end(), pRecipient ) ==  recipients_.end() )
        recipients_.push_back( pRecipient );

    route_->AddRecipient( *pRecipient );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::RemoveRecipient
// Created: MMC 2012-10-12
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::RemoveRecipient( const QString& recipient )
{
    EraseRecipientData( recipient );
    pRecipientSelected_ = 0;
    resourcesTable_->Clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Clear
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Clear()
{
    recipientsList_->Clear();
    pRecipientSelected_ = 0;
    ClearRecipientsData();
    resourcesTable_->Clear();
    availableSupplies_.clear();
    carriersTable_->Clear();
    ClearCarriersData();
    selected_ = 0;
    route_->Clear();
    EnableButtons( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearRecipientsData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearRecipientsData()
{
    recipients_.clear();
    recipientSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::EraseRecipientData
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::EraseRecipientData( const QString& recipient )
{
    const kernel::Automat_ABC* pRecipient = recipientsNames_[ recipient ];
    recipients_.erase( std::remove( recipients_.begin(), recipients_.end(), pRecipient ), recipients_.end() );
    if( pRecipient )
        route_->RemoveRecipient( *pRecipient );

    for( auto itSupplies = recipientSupplies_.begin(); itSupplies != recipientSupplies_.end(); )
    {
        if( itSupplies->first == pRecipient )
            itSupplies = recipientSupplies_.erase( itSupplies );
        else
            ++itSupplies;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::computeAvailableCarriers
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeAvailableCarriers( QMap< QString, int >& availableCarriers )
{
    carriersTypes_.clear();
    carriersTypeNames_.clear();
    AddCarryingEquipment( *selected_ );
    for( auto it = carriersTypes_.begin(); it != carriersTypes_.end(); ++it )
        if( !it.key().isEmpty() && it.value() > 0 )
            availableCarriers[ it.key() ] = static_cast< int >( it.value() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::GetSuppliesFromTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::GetSuppliesFromTable( const kernel::Automat_ABC& recipient )
{
    auto it = recipientSupplies_.find( &recipient );
    if( it != recipientSupplies_.end() )
        resourcesTable_->GetQuantities( it->second );
}

namespace
{
    bool HasDotation( const kernel::Automat_ABC& recipient, const QString& dotationName )
    {
        const Dotations* dotations = static_cast< const Dotations* >( recipient.Retrieve< kernel::Dotations_ABC >() );
        if( dotations )
        {
            const std::string strDotationName = dotationName.toStdString();
            auto it = dotations->CreateIterator();
            while( it.HasMoreElements() )
            {
                const Dotation& dotation = it.NextElement();
                if( dotation.type_ && dotation.type_->GetName() == strDotationName )
                    return true;
            }
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::SetSuppliesToTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::SetSuppliesToTable( const kernel::Automat_ABC& recipient )
{
    QMap< QString, int > maxQuantities;
    for( auto it = availableSupplies_.begin(); it != availableSupplies_.end(); ++it )
        if( !it->first.isEmpty() && it->second.type_ && it->second.quantity_ > 0 )
            if( isPushFlow_ || HasDotation( recipient, it->first ) )
            {
                int quantity = it->second.quantity_;
                // remove from available quantity the dotations that are already allocated
                for( auto recIt = recipientSupplies_.begin(); recIt != recipientSupplies_.end(); ++recIt )
                {
                    if( recIt->first == &recipient )
                        continue;
                    const T_QuantitiesMap& quantities = recIt->second;
                    for( auto qIt = quantities.begin(); qIt != quantities.end(); ++qIt )
                        if( qIt.key() == it->first )
                            quantity = std::max( 0, quantity - qIt.value() );
                }
                maxQuantities[ it->first ] = quantity;
            }
    resourcesTable_->SetQuantities( recipientSupplies_[ &recipient ], maxQuantities );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::OnRecipientSelectionChanged
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnRecipientSelectionChanged( const QString& currentRecipient, const QString& previousRecipient )
{
    if( !previousRecipient.isEmpty() )
    {
        const kernel::Automat_ABC* pPreviousRecipient = recipientsNames_[ previousRecipient ];
        if( pPreviousRecipient )
            GetSuppliesFromTable( *pPreviousRecipient );
    }
    resourcesTable_->Clear();
    if( !currentRecipient.isEmpty() )
    {
        pRecipientSelected_ = recipientsNames_[ currentRecipient ];
        if( pRecipientSelected_ )
            SetSuppliesToTable( *pRecipientSelected_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::ComputeRecipients
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeRecipients()
{
    recipientsNames_.clear();
    auto it = automats_.CreateIterator();
    const kernel::Entity_ABC& team = selected_->Get< kernel::TacticalHierarchies >().GetTop();
    while( it.HasMoreElements() )
    {
        const kernel::Automat_ABC& automat = it.NextElement();
        if( &automat != selected_ )
        {
            bool add = false;
            if( isPushFlow_)
            {
                const kernel::AutomatType& type = automat.Get< gui::EntityType< kernel::AutomatType > >().GetType();
                add = type.IsLogisticSupply() && &automat.Get< kernel::TacticalHierarchies >().GetTop() == &team;
            }
            else
                add = automat.Get< LogisticLinks >().GetCurrentSuperior() == selected_;
            if( add )
                recipientsNames_[ automat.GetName() ] = &automat;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::computeAvailableRecipients
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeAvailableRecipients( QStringList& displayRecipientsNames )
{
    displayRecipientsNames.clear();
    for( auto it = recipientsNames_.begin(); it != recipientsNames_.end(); ++it )
        if( std::find( recipients_.begin(), recipients_.end(), it.value() ) == recipients_.end() )
            displayRecipientsNames.append( it.key() );
}
