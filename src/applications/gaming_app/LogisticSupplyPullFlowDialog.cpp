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
#include "LogisticSupplyCarriersTableWidget.h"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "actions/PullFlowParameters.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Availability.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Location_ABC.h"
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

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog constructor
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
LogisticSupplyPullFlowDialog::LogisticSupplyPullFlowDialog( QWidget* parent,
                                                            Controllers& controllers,
                                                            ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel,
                                                            const Time_ABC& simulation,
                                                            ParametersLayer& layer,
                                                            const tools::Resolver_ABC< Automat_ABC >& automats,
                                                            const tools::Resolver_ABC< Formation_ABC >& formations,
                                                            const kernel::Profile_ABC& profile )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, layer, automats )
    , formations_( formations )
    , supplier_( 0 )
    , profile_( profile )
{
    setCaption( tr( "Pull supply flow" ) );
    supplierCombo_ = new ValuedComboBox< const Entity_ABC* >( "supplierCombo", resourcesTab_ );
    supplierCombo_->setMinimumWidth( 260 );
    connect( supplierCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSupplierValueChanged() ) );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Show
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Show( const kernel::Entity_ABC& entity )
{
    selected_ = &entity;

    controllers_.Update( *routeLocationCreator_ );
    routeLocationCreator_->StartLine();

    tabs_->setCurrentPage( 0 );
    delWaypointButton_->setEnabled( false );
    moveUpButton_->setEnabled( false );
    moveDownButton_->setEnabled( false );

    ComputeAvailableSuppliers();

    supplierCombo_->clear();
    supplierCombo_->AddItem( QString(), 0 );
    for( auto it = suppliersNames_.begin(); it != suppliersNames_.end(); ++it )
        supplierCombo_->AddItem( it.key(), it.value() );

    OnSupplierValueChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::SetSuppliesToTable
// Created: MMC 2012-10-16
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::SetSuppliesToTable()
{
    QMap< QString, int > maxQuantities, quantities;
    for( auto it = availableSupplies_.begin(); it != availableSupplies_.end(); ++it )
        if( !it->first.isEmpty() && it->second.type_ && it->second.quantity_ > 0 )
            maxQuantities[ it->first ] = it->second.quantity_;
     resourcesTable_->SetQuantities( quantities, maxQuantities );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Clear
// Created: SLI 2014-01-10
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Clear()
{
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
// Name: LogisticSupplyPullFlowDialog::Validate
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Validate()
{
    if( !selected_ || !supplier_ )
        return;
    if( profile_.IsSupervision() )
    {
        if( carriersTable_->IsIncomplete() )
        {
            QMessageBox::critical( this, tr( "Error" ), tr( "The convoy is unable to carry all types of resources" ) );
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

    T_QuantitiesMap supplies;
    resourcesTable_->GetQuantities( supplies );
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
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "log_supply_pull_flow" );
    std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

    PullFlowParameters* pullFlowParameters = new PullFlowParameters( it.NextElement(), static_.coordinateConverter_ );
    const Automat_ABC* pSupplierAutomat = dynamic_cast< const Automat_ABC* >( supplier_ );
    const Formation_ABC* pSupplierFormation = dynamic_cast< const Formation_ABC* >( supplier_ );
    assert( pSupplierAutomat || pSupplierFormation );
    if( pSupplierAutomat )
        pullFlowParameters->SetSupplier( *pSupplierAutomat );
    else if( pSupplierFormation )
        pullFlowParameters->SetSupplier( *pSupplierFormation );

    for( auto it = supplies.begin(); it != supplies.end(); ++it )
    {
        const DotationType* dotationType = availableSupplies_[ it.key() ].type_;
        assert( dotationType );
        if( dotationType )
            pullFlowParameters->AddResource( *dotationType, it.value() );
    }
    for( auto it = carriers.begin(); it != carriers.end(); ++it )
        pullFlowParameters->AddTransporter( *carriersTypeNames_[ it.key() ], it.value() );

    // Route
    CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    T_PointVector outPath, backPath;
    bool isOutPath = true;
    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
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
    action->Attach( *new ActionTasker( controllers_.controller_, selected_, false ) );
    actionsModel_.Publish( *action );

    Clear();
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
    Clear();
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
}

namespace
{
    typedef std::map< std::size_t, std::size_t > T_Carriers;
    T_Carriers GetAvailableCarriers( const kernel::Entity_ABC& supplier )
    {
        T_Carriers result;
        logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( supplier, [&]( const kernel::Entity_ABC& entity )
        {
            if( const SupplyStates* pState = entity.Retrieve< SupplyStates >() )
                for( auto it = pState->dispoTransporters_.begin(); it != pState->dispoTransporters_.end(); ++it )
                    result[ it->type_->GetId() ] += it->available_;
        } );
        return result;
    }
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
    AddCarryingEquipment( *selected_ );
    const T_Carriers carriers = GetAvailableCarriers( *selected_ );
    for( auto it = carriersTypes_.begin(); it != carriersTypes_.end(); ++it )
    {
        auto carrier = carriers.find( carriersTypeNames_[ it.key() ]->GetId() );
        if( !it.key().isEmpty() && it.value() > 0 && carrier != carriers.end() )
            availableCarriers[ it.key() ] = static_cast< int >( carrier->second );
    }
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
    ClearSuppliersData();

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
    supplier_ = selection.isEmpty() ? 0 : suppliersNames_[ selection ];
    availableSupplies_.clear();
    resourcesTable_->Clear();
    if( !supplier_ )
        return;
    logistic_helpers::VisitBaseStocksDotations( *supplier_, boost::bind( &LogisticSupplyPullFlowDialog::AddAvailable, this, _1 ) );
    SetSuppliesToTable();
}

namespace
{
    template< typename Elements, typename Suppliers, typename Selected >
    void FillSuppliers( const Elements& elements, Suppliers& suppliers, const Selected& selected )
    {
        const Entity_ABC& team = selected->Get< TacticalHierarchies >().GetTop();
        auto it = elements.CreateIterator();
        while( it.HasMoreElements() )
        {
            const auto& element = it.NextElement();
            if( &element != selected &&
                element.Get< LogisticBase >().IsBase() &&
                &element.Get< TacticalHierarchies >().GetTop() == &team )
                    suppliers[ element.GetName() ] = &element;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers()
{
    suppliersNames_.clear();
    FillSuppliers( automats_, suppliersNames_, selected_ );
    FillSuppliers( formations_, suppliersNames_, selected_ );
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

    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
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
    {
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
    }
    routeDrawpoints_.push_back( startPos );
}
