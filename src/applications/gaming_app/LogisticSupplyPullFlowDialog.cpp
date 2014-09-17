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
#include "LogisticRouteWidget.h"
#include "LogisticSupplyCarriersTableWidget.h"

#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ActionsModel.h"
#include "actions/PullFlowParameters.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Availability.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
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
                                                            const tools::Resolver_ABC< Automat_ABC >& automats,
                                                            const tools::Resolver_ABC< Formation_ABC >& formations,
                                                            const kernel::Profile_ABC& profile,
                                                            const gui::EntitySymbols& symbols )
    : LogisticSupplyFlowDialog_ABC( parent, controllers, actionsModel, staticModel, simulation, automats, profile, symbols )
    , formations_( formations )
    , supplier_( 0 )
    , lastContext_( 0 )
{
    setCaption( tr( "Pull supply flow" ) );
    supplierCombo_ = new ValuedComboBox< const Entity_ABC* >( "supplierCombo", resourcesTab_ );
    supplierCombo_->setMinimumWidth( 260 );
    connect( supplierCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSupplierValueChanged() ) );
    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab_ );
    resourcesLayout->addWidget( supplierCombo_ );
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

    tabs_->setCurrentPage( 0 );
    ComputeAvailableSuppliers();
    supplierCombo_->clear();
    supplierCombo_->AddItem( QString(), 0 );
    for( auto it = suppliersNames_.begin(); it != suppliersNames_.end(); ++it )
        supplierCombo_->AddItem( it.key(), it.value() );

    OnSupplierValueChanged();
    route_->AddRequester( selected_ );
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
    selected_ = 0;
    route_->Clear();
    EnableButtons( true );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Validate
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Validate()
{
    if( !selected_ || !supplier_ )
        return;
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

    T_QuantitiesMap supplies;
    resourcesTable_->GetQuantities( supplies );
    T_QuantitiesMap carriers;
    if( carriersUseCheck_->isChecked() )
        carriersTable_->GetQuantities( carriers );

    if( !selected_ )
        return Reject();

    EnableButtons( false );

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

    route_->FillPullFlowParameters( *pullFlowParameters );

    action->AddParameter( *pullFlowParameters );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( controllers_.controller_, selected_, false ) );
    lastContext_ = actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Reject
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Reject()
{
    reject();
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
    if( supplier_ )
        route_->RemoveRecipient( *supplier_ );

    ClearSuppliersData();
    QString selection = supplierCombo_->text( supplierCombo_->currentIndex() );
    supplier_ = suppliersNames_[ selection ];

    QMap< QString, int > carriersQty, carriersAvailable;
    carriersUseCheck_->setCheckState( Qt::Unchecked );
    ComputeAvailableCarriers( carriersAvailable );
    carriersTable_->SetQuantities( carriersQty, carriersAvailable );
    carriersTable_->setEnabled( false );
    OnSupplierSelectionChanged();

    if( supplier_ )
        route_->AddRecipient( *supplier_ );
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
    logistic_helpers::VisitBaseStocksDotations( *supplier_, [&]( const Dotation& dotation ){ AddAvailable( dotation ); } );
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
