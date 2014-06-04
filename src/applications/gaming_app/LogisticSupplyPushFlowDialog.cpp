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
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "LogisticSupplyExclusiveListWidget.h"
#include "LogisticSupplyCarriersTableWidget.h"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/PushFlowParameters.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/EntityType.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "gaming/Dotations.h"
#include "gaming/LogisticLinks.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent,
                                                            kernel::Controllers& controllers,
                                                            actions::ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel,
                                                            const kernel::Time_ABC& simulation,
                                                            gui::ParametersLayer& layer,
                                                            const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                                            const kernel::Profile_ABC& profile )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, layer, automats, profile )
    , isPushFlow_( true )
{
    recipientsList_ = new LogisticSupplyExclusiveListWidget( this, tr( "Add recipient" ), tr( "Remove recipient" ) );
    connect( recipientsList_, SIGNAL( ItemAdded( const QString& ) ), SLOT( AddRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( ItemRemoved( const QString& ) ), SLOT( RemoveRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( SelectionChanged( const QString&, const QString& ) ),
                              SLOT( OnRecipientSelectionChanged( const QString&, const QString& ) ) );
    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab_ );
    resourcesLayout->addWidget( recipientsList_ );
    resourcesLayout->addWidget( resourcesTable_ );
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
    controllers_.Update( *routeLocationCreator_ );
    routeLocationCreator_->StartLine();

    logistic_helpers::VisitPartialBaseStocksDotations( *selected_,
                                                       [&]( const Dotation& dotation ){ AddAvailable( dotation ); },
                                                       [&]( const kernel::Entity_ABC& entity ){ return profile_.CanBeOrdered( entity ); } );

    ComputeRecipients();
    tabs_->setCurrentPage( 0 );
    delWaypointButton_->setEnabled( false );
    moveUpButton_->setEnabled( false );
    moveDownButton_->setEnabled( false );

    QStringList recipientsList;
    ComputeAvailableRecipients( recipientsList );
    recipientsList_->SetChoice( recipientsList );

    QMap< QString, int > carriersQty, carriersAvailable;
    carriersUseCheck_->setCheckState( Qt::Unchecked );
    ComputeAvailableCarriers( carriersAvailable );
    carriersTable_->SetQuantities( carriersQty, carriersAvailable );
    carriersTable_->setEnabled( false );

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

    accept();
    layer_.Reset();
    controllers_.Unregister( *waypointLocationCreator_ );
    controllers_.Unregister( *routeLocationCreator_ );

    if( !selected_ )
        return;

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

    // Route
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    T_PointVector currentPath;
    const kernel::Automat_ABC* currentRecipient = 0;
    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        const QString str = *it;
        if( points_.find( str ) != points_.end() )
            currentPath.push_back( points_[ str ] );
        else
        {
            currentRecipient = recipientsNames_[ str ];
            pushFlowParameters->SetPath( currentPath, *currentRecipient );
            currentPath.clear();
        }
    }
    pushFlowParameters->SetWayBackPath( currentPath );

    action->AddParameter( *pushFlowParameters );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );

    Clear();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    reject();
    layer_.Reset();
    controllers_.Unregister( *waypointLocationCreator_ );
    controllers_.Unregister( *routeLocationCreator_ );

    Clear();
    selected_ = 0;
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

    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    waypoints.append( recipientName );
    pModel->setStringList( waypoints );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::RemoveRecipient
// Created: MMC 2012-10-12
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::RemoveRecipient( const QString& recipient )
{
    EraseRecipientData( recipient );
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    waypoints.removeAll( recipient );
    pModel->setStringList( waypoints );
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
    ClearRouteList();
    ClearRouteData();
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
                maxQuantities[ it->first ] = it->second.quantity_;
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
                recipientsNames_[ automat.GetName() + QString( " [" ) + QString::number( automat.GetId() ) + QString( "]" ) ] = &automat;
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

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::ComputeRoute
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeRoute( T_Route& route )
{
    route.clear();
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();

    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        const QString str = *it;
        if( points_.find( str ) != points_.end() )
            route.push_back( Waypoint( points_[ str ] ) );
        else if( recipientsNames_.find( str ) != recipientsNames_.end() )
            route.push_back( Waypoint( recipientsNames_[ str ] ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::UpdateRouteDrawpoints
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::UpdateRouteDrawpoints()
{
    if( !selected_ )
        return;

    routeDrawpoints_.clear();
    T_Route route;
    ComputeRoute( route );

    geometry::Point2f startPos = logistic_helpers::GetLogisticPosition( *selected_, true );
    if( startPos.IsZero() )
        return;

    routeDrawpoints_.push_back( startPos );

    for( std::size_t i = 0; i < route.size(); ++i )
        if( route[ i ].IsPoint() )
            routeDrawpoints_.push_back( route[ i ].point_ );
        else
        {
            if( route[ i ].pRecipient_ )
            {
                geometry::Point2f pos = logistic_helpers::GetLogisticPosition( *route[ i ].pRecipient_, true );
                if( !pos.IsZero() )
                    routeDrawpoints_.push_back( pos );
            }
        }

    routeDrawpoints_.push_back( startPos );
}
