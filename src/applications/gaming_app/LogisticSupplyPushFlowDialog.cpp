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

#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/PushFlowParameters.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/SimpleLocationDrawer.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "protocol/SimulationSenders.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace longname;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel, const Time_ABC& simulation,
                                                            ParametersLayer& layer, const tools::Resolver_ABC< Automat_ABC >& automats,
                                                            const Profile_ABC& profile )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, layer, automats, profile )
{
    setCaption( tr( "Push supply flow" ) );

    recipientsList_ = new LogisticSupplyExclusiveListWidget( this, tr( "Add recipient" ), tr( "Remove recipient" ) );
    connect( recipientsList_, SIGNAL( ItemAdded( const QString& ) ), SLOT( AddRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( ItemRemoved( const QString& ) ), SLOT( RemoveRecipient( const QString& ) ) );
    connect( recipientsList_, SIGNAL( SelectionChanged( const QString&, const QString& ) ),
                              SLOT( OnRecipientSelectionChanged( const QString&, const QString& ) ) );
    QStringList resourcesHeader;
    resourcesHeader << tools::translate( "Logistic : Push supply flow", "Resource" )
        << tools::translate( "Logistic : Push supply flow", "Available" )
        << tools::translate( "Logistic : Push supply flow", "Quantity" );
    resourcesTable_ = new LogisticSupplyAvailabilityTableWidget( this, resourcesHeader );

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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Formation_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::InsertMenuEntry
// Created: ABR 2011-06-29
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::InsertMenuEntry( const Entity_ABC& agent, ContextMenu& menu )
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

    controllers_.Update( *routeLocationCreator_ );
    routeLocationCreator_->StartLine();

    logistic_helpers::VisitBaseStocksDotations( *selected_, boost::bind( &LogisticSupplyPushFlowDialog::AddAvailable, this, _1 ) );

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
    if( pRecipientSelected_ )
        GetSuppliesFromTable( *pRecipientSelected_ );
    if( carriersUseCheck_->isChecked() )
        GetCarriersFromTable();

    accept();
    layer_.Reset();
    controllers_.Unregister( *waypointLocationCreator_ );
    controllers_.Unregister( *routeLocationCreator_ );

    if( !selected_ )
        return;

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( dynamic_cast< const Automat_ABC* >( static_cast< const Entity_ABC* >( selected_ ) ) ) ? "automat_log_supply_push_flow" : "formation_log_supply_push_flow" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Log Supply Push Flow" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

    PushFlowParameters* pushFlowParameters = new PushFlowParameters( it.NextElement(), static_.coordinateConverter_ );
    BOOST_FOREACH( const T_RecipientSupplies::value_type& recipientSupply, recipientSupplies_ )
    {
        const T_QuantitiesMap& supplyQuantities = recipientSupply.second;
        for( T_QuantitiesMap::const_iterator it = supplyQuantities.begin(); it != supplyQuantities.end(); ++it )
        {
            const DotationType* dotationType = availableSupplies_[ it.key() ].type_;
            assert( dotationType );
            if( dotationType )
                pushFlowParameters->AddResource( *dotationType, it.value(), *recipientSupply.first );
        }
    }
    if( carriersUseCheck_->isChecked() )
        for( T_QuantitiesMap::iterator it = carriers_.begin(); it != carriers_.end(); ++it )
            pushFlowParameters->AddTransporter( *carriersTypeNames_[ it.key() ], it.value() );

    // Route
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    T_PointVector currentPath;
    const Automat_ABC* currentRecipient = 0;
    for( QStringList::iterator it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        QString str = *it;
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
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );

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
// Name: LogisticSupplyPushFlowDialog::AddRecipientItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddRecipient( const QString& recipientName )
{
    const Automat_ABC* pRecipient = recipientsNames_[ recipientName ];
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
    const Automat_ABC* pRecipient = recipientsNames_[ recipient ];
    recipients_.erase( std::remove( recipients_.begin(), recipients_.end(), pRecipient ), recipients_.end() );

    T_RecipientSupplies::iterator itSupplies = recipientSupplies_.begin();
    while( itSupplies != recipientSupplies_.end() )
    {
        T_RecipientSupplies::iterator curItSupplies = itSupplies++;
        if( curItSupplies->first == pRecipient )
            recipientSupplies_.erase( curItSupplies );
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
    const TacticalHierarchies* pTacticalHierarchies = selected_->Retrieve< TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;
    AddCarryingEquipment( *selected_ );
    tools::Iterator< const Entity_ABC& > itEnt = pTacticalHierarchies->CreateSubordinateIterator();
    while( itEnt.HasMoreElements() )
        AddCarryingEquipment( itEnt.NextElement() );
    for( T_QuantitiesMap::const_iterator it = carriersTypes_.begin(); it != carriersTypes_.end(); ++it )
        if( !it.key().isEmpty() && it.value() > 0 )
            availableCarriers[ it.key() ] = static_cast< int >( it.value() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::GetSuppliesFromTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::GetSuppliesFromTable( const kernel::Automat_ABC& recipient )
{
    T_RecipientSupplies::iterator it = recipientSupplies_.find( &recipient );
    if( it != recipientSupplies_.end() )
    {
        T_QuantitiesMap& curSupplies = it->second;
        resourcesTable_->GetQuantities( curSupplies );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::SetSuppliesToTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::SetSuppliesToTable( const kernel::Automat_ABC& recipient )
{
    QMap< QString, int > maxQuantities, quantities;
    for( T_Supplies::iterator it = availableSupplies_.begin(); it != availableSupplies_.end(); ++it )
        if( !it->first.isEmpty() && it->second.type_ && it->second.quantity_ > 0 )
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
    tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
    const Entity_ABC& team = selected_->Get< TacticalHierarchies >().GetTop();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        if( (const Entity_ABC*)&automat != selected_ )
        {
            const AutomatType& type = automat.Get< kernel::EntityType< kernel::AutomatType > >().GetType();
            if( type.IsLogisticSupply() && &automat.Get< TacticalHierarchies >().GetTop() == &team )
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
    for( T_RecipientsNames::iterator it = recipientsNames_.begin(); it != recipientsNames_.end(); ++it )
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

    for( QStringList::iterator it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        QString str = *it;
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
    routeDrawpoints_.clear();
    T_Route route;
    ComputeRoute( route );

    geometry::Point2f startPos;
    if( selected_ )
        startPos = logistic_helpers::GetLogisticPosition( *selected_, true );
    if( !startPos.IsZero() )
        routeDrawpoints_.push_back( startPos );

    for( std::size_t i = 0; i < route.size(); ++i )
        if( route[i].IsPoint() )
            routeDrawpoints_.push_back( route[i].point_ );
        else
        {
            geometry::Point2f pos;
            if( route[i].pRecipient_ )
                pos = logistic_helpers::GetLogisticPosition( *route[i].pRecipient_, true );
            if( !pos.IsZero() )
                routeDrawpoints_.push_back( pos );
        }

    routeDrawpoints_.push_back( startPos );
}
