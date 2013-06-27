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
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "actions/PullFlowParameters.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog constructor
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
LogisticSupplyPullFlowDialog::LogisticSupplyPullFlowDialog( QWidget* parent, Controllers& controllers, ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel, const Time_ABC& simulation,
                                                            ParametersLayer& layer, const tools::Resolver_ABC< Automat_ABC >& automats,
                                                            const tools::Resolver_ABC< Formation_ABC >& formations,
                                                            const Profile_ABC& profile )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, layer, automats, profile )
    ,  formations_( formations )
    , supplier_( 0 )
{
    setCaption( tr( "Pull supply flow" ) );

    supplierCombo_ = new ValuedComboBox< const Entity_ABC* >( "supplierCombo", resourcesTab_ );
    supplierCombo_->setMinimumWidth( 260 );
    connect( supplierCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSupplierValueChanged() ) );

    QStringList resourcesHeader;
    resourcesHeader << tools::translate( "Logistic : Push supply flow", "Resource" )
        << tools::translate( "Logistic : Push supply flow", "Available" )
        << tools::translate( "Logistic : Push supply flow", "Quantity" );
    resourcesTable_ = new LogisticSupplyAvailabilityTableWidget( this, resourcesHeader );

    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab_ );
    resourcesLayout->addWidget( supplierCombo_ );
    resourcesLayout->addWidget( resourcesTable_ );
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
        if( agent.GetLogisticLevel() != LogisticLevel::none_ )
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

    controllers_.Update( *routeLocationCreator_ );
    routeLocationCreator_->StartLine();

    tabs_->setCurrentPage( 0 );
    delWaypointButton_->setEnabled( false );
    moveUpButton_->setEnabled( false );
    moveDownButton_->setEnabled( false );

    ComputeAvailableSuppliers();

    supplierCombo_->clear();
    supplierCombo_->AddItem( QString(), 0 );
    for( T_SuppliersNames::iterator it = suppliersNames_.begin(); it != suppliersNames_.end(); ++it )
        supplierCombo_->AddItem( it.key(), it.value() );

    OnSupplierValueChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::GetSuppliesFromTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::GetSuppliesFromTable()
{
    resourcesTable_->GetQuantities( supplierSupplies_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::SetSuppliesToTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::SetSuppliesToTable()
{
    QMap< QString, int > maxQuantities, quantities;
    for( T_AvailableDotations::const_iterator it = availableSupplies_.begin(); it != availableSupplies_.end(); ++it )
        if( !it->first.isEmpty() && it->second.type_ && it->second.quantity_ > 0 )
            maxQuantities[ it->first ] = it->second.quantity_;
     resourcesTable_->SetQuantities( quantities, maxQuantities );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Validate
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Validate()
{
    if( !selected_ || !supplier_ )
        return;

    GetSuppliesFromTable();
    if( carriersUseCheck_->isChecked() )
        GetCarriersFromTable();

    accept();
    layer_.Reset();
    controllers_.Unregister( *waypointLocationCreator_ );
    controllers_.Unregister( *routeLocationCreator_ );

    if( !selected_ )
        return;

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( dynamic_cast< const Automat_ABC* >( static_cast< const Entity_ABC* >( selected_ ) ) ) ? "automat_log_supply_pull_flow" : "formation_log_supply_pull_flow" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Log Supply Pull Flow" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

    PullFlowParameters* pullFlowParameters = new PullFlowParameters( it.NextElement(), static_.coordinateConverter_ );
    const Automat_ABC* pSupplierAutomat = dynamic_cast< const Automat_ABC* >( supplier_ );
    const Formation_ABC* pSupplierFormation = dynamic_cast< const Formation_ABC* >( supplier_ );
    assert( pSupplierAutomat || pSupplierFormation );
    if( pSupplierAutomat )
        pullFlowParameters->SetSupplier( *pSupplierAutomat );
    else if( pSupplierFormation )
        pullFlowParameters->SetSupplier( *pSupplierFormation );

    for( T_QuantitiesMap::iterator it = supplierSupplies_.begin(); it != supplierSupplies_.end(); ++it )
    {
        const DotationType* dotationType = availableSupplies_[ it.key() ].type_;
        assert( dotationType );
        if( dotationType )
            pullFlowParameters->AddResource( *dotationType, it.value() );
    }

    if( carriersUseCheck_->isChecked() )
        for( T_QuantitiesMap::iterator it = carriers_.begin(); it != carriers_.end(); ++it )
            pullFlowParameters->AddTransporter( *carriersTypeNames_[ it.key() ], it.value() );

    // Route
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    T_PointVector outPath, backPath;
    bool isOutPath = true;
    for( QStringList::iterator it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        QString str = *it;
        if( points_.find( str ) != points_.end() )
        {
            if( isOutPath )
                outPath.push_back( points_[ str ] );
            else
                backPath.push_back( points_[ str ] );
        }
        else
            isOutPath = false;
    }
    pullFlowParameters->SetWayOutPath( outPath );
    pullFlowParameters->SetWayBackPath( backPath );

    action->AddParameter( *pullFlowParameters );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );

    ClearSuppliersTable();
    ClearSuppliersData();
    resourcesTable_->Clear();
    availableSupplies_.clear();
    carriersTable_->Clear();
    ClearCarriersData();
    ClearRouteList();
    ClearRouteData();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Reject
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Reject()
{
    reject();
    layer_.Reset();
    controllers_.Unregister( *waypointLocationCreator_ );
    controllers_.Unregister( *routeLocationCreator_ );

    ClearSuppliersTable();
    ClearSuppliersData();
    resourcesTable_->Clear();
    availableSupplies_.clear();
    carriersTable_->Clear();
    ClearCarriersData();
    ClearRouteList();
    ClearRouteData();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearSuppliersTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearSuppliersTable()
{
    supplierCombo_->setCurrentText( QString() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearSuppliersData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearSuppliersData()
{
    supplier_ = 0;
    supplierSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::EraseSupplierData
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::EraseSupplierData()
{
    supplier_ = 0;
    supplierSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::computeAvailableCarriers
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeAvailableCarriers( QMap< QString, int >& availableCarriers )
{
    if( !supplier_ )
        return;
    carriersTypes_.clear();
    carriersTypeNames_.clear();
    const TacticalHierarchies* pTacticalHierarchies = supplier_->Retrieve< TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;
    AddCarryingEquipment( *supplier_ );
    tools::Iterator< const Entity_ABC& > itEnt = pTacticalHierarchies->CreateSubordinateIterator();
    while( itEnt.HasMoreElements() )
        AddCarryingEquipment( itEnt.NextElement() );
    for( T_QuantitiesMap::const_iterator it = carriersTypes_.begin(); it != carriersTypes_.end(); ++it )
        if( !it.key().isEmpty() && it.value() > 0 )
            availableCarriers[ it.key() ] = static_cast< int >( it.value() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSupplierValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnSupplierValueChanged()
{
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    if( supplier_ )
        waypoints.removeAll( supplier_->GetName() );
    EraseSupplierData();

    QString selection = supplierCombo_->text( supplierCombo_->currentIndex() );
    supplier_ = suppliersNames_[ selection ];
    if( supplier_ )
        waypoints.append( supplier_->GetName() );

    QMap< QString, int > carriersQty, carriersAvailable;
    carriersUseCheck_->setCheckState( Qt::Unchecked );
    ComputeAvailableCarriers( carriersAvailable );
    carriersTable_->SetQuantities( carriersQty, carriersAvailable );
    carriersTable_->setEnabled( false );

    pModel->setStringList( waypoints );
    OnSupplierSelectionChanged();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSupplierSelectionChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnSupplierSelectionChanged()
{
    QString selection =  supplierCombo_->text( supplierCombo_->currentIndex() );
    if( selection.isEmpty() )
        supplier_ = 0;
    else
        supplier_ = suppliersNames_[ selection ];

    if( !supplier_ )
    {
        availableSupplies_.clear();
        resourcesTable_->Clear();
    }
    else
    {
        availableSupplies_.clear();
        resourcesTable_->Clear();
        logistic_helpers::VisitBaseStocksDotations( *supplier_, boost::bind( &LogisticSupplyPullFlowDialog::AddAvailable, this, _1 ) );
        SetSuppliesToTable();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers()
{
    suppliersNames_.clear();

    const Entity_ABC& team = selected_->Get< TacticalHierarchies >().GetTop();
    {
        tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& automat = it.NextElement();
            if( &automat != selected_ )
            {
                if( automat.GetLogisticLevel() != LogisticLevel::none_ && &automat.Get< TacticalHierarchies >().GetTop() == &team )
                    suppliersNames_[ automat.GetName() ] = &automat;
            }
        }
    }
    {
        tools::Iterator< const Formation_ABC& > it = formations_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& formation = it.NextElement();
            if( formation.GetLogisticLevel() != LogisticLevel::none_ && &formation.Get< TacticalHierarchies >().GetTop() == &team )
                suppliersNames_[ formation.GetName() ] = &formation;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ComputeRoute
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeRoute( T_Route& route )
{
    route.clear();
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();

    for( QStringList::iterator it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        QString str = *it;
        if( points_.find( str ) != points_.end() )
            route.push_back( Waypoint( points_[ str ] ) );
        else if( supplier_ )
            route.push_back( Waypoint( supplier_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::UpdateRouteDrawpoints
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::UpdateRouteDrawpoints()
{
    routeDrawpoints_.clear();
    T_Route route;
    ComputeRoute( route );

    if( !supplier_ )
        return;

    geometry::Point2f startPos;
    if( selected_ )
        startPos = logistic_helpers::GetLogisticPosition( *selected_, true );
    if( !startPos.IsZero() )
        routeDrawpoints_.push_back( startPos );

    for( std::size_t i=0; i < route.size(); ++i )
        if( route[i].IsPoint() )
            routeDrawpoints_.push_back( route[i].point_ );
        else
        {
            geometry::Point2f pos;
            if( supplier_ )
                pos = logistic_helpers::GetLogisticPosition( *supplier_, true );
            if( !pos.IsZero() )
                routeDrawpoints_.push_back( pos );
        }

    routeDrawpoints_.push_back( startPos );
}

