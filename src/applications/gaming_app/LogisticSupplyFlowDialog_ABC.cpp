// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyFlowDialog_ABC.h"
#include "moc_LogisticSupplyFlowDialog_ABC.cpp"
#include "LogisticRouteWidget.h"
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "LogisticSupplyCarriersTableWidget.h"

#include "clients_gui/EntityType.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/resources.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace longname;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyFlowDialog_ABC::LogisticSupplyFlowDialog_ABC( QWidget* parent,
                                                            kernel::Controllers& controllers,
                                                            actions::ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel,
                                                            const kernel::Time_ABC& simulation,
                                                            const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                                            const kernel::Profile_ABC& profile,
                                                            const gui::EntitySymbols& symbols )
    : QDialog( parent, tr( "Supply flow" ), 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , automats_( automats )
    , selected_( controllers )
    , profile_( profile )
    , route_( new LogisticRouteWidget( controllers, symbols ) )
{
    tabs_ = new QTabWidget( this );
    tabs_->setMargin( 5 );
    QGridLayout* tabLayout = new QGridLayout( this, 1, 2 );
    tabLayout->addWidget( tabs_, 0, 0, 1, 3 );

    QStringList resourcesHeader;
    resourcesHeader << tr( "Supplies" )
        << tr( "Available" )
        << tr( "Quantity" )
        << tr( "Mass (T)" )
        << tr( "Volume (m3)" );
    resourcesTable_ = new LogisticSupplyAvailabilityTableWidget( this, resourcesHeader, availableSupplies_ );

    resourcesTab_ = new QWidget( tabs_ );
    QWidget* carriersTab = new QWidget( tabs_ );
    tabs_->addTab( resourcesTab_, tr( "Supplies" ) );
    tabs_->addTab( carriersTab, tr( "Carriers" ) );
    tabs_->addTab( route_, tr( "Route" ) );

    cancel_ = new QPushButton( tr( "Cancel" ), tabs_ );
    ok_ = new QPushButton( tr( "Ok" ), tabs_ );
    connect( cancel_, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( ok_, SIGNAL( clicked() ), SLOT( Validate() ) );

    tabLayout->addWidget( ok_, 1, 0, 1, 1 );
    tabLayout->addWidget( cancel_, 1, 2, 1, 1 );
    setMinimumSize( 750, 420 );
    tabLayout->setMargin( 5 );
    tabLayout->setSpacing( 5 );

    carriersUseCheck_ = new QCheckBox( tr( "Manual selection of transport carriers" ), carriersTab );
    connect( carriersUseCheck_, SIGNAL( stateChanged( int ) ), this, SLOT( OnCarriersUseCheckStateChanged() ) );
    QStringList carriersHeader;
    carriersHeader << tr( "Type" )
        << tr( "Available" )
        << tr( "Quantity" )
        << tr( "Capacity (T)" )
        << tr( "Capacity (m3)" )
        << tr( "Mass" )
        << tr( "Volume" );
    carriersTable_ = new LogisticSupplyCarriersTableWidget( this, carriersHeader, carriersTypeNames_, *resourcesTable_, availableSupplies_ );
    carriersTable_->setEnabled( false );
    connect( resourcesTable_, SIGNAL( OnChanged( int ) ), carriersTable_, SLOT( Update() ) );

    QVBoxLayout* carriersLayout = new QVBoxLayout( carriersTab );
    carriersLayout->addWidget( carriersUseCheck_ );
    carriersLayout->addWidget( carriersTable_ );
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC destructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyFlowDialog_ABC::~LogisticSupplyFlowDialog_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::closeEvent
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::AddAvailable
// Created : MMC 2012-10-15
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::AddAvailable( const Dotation& dotation )
{
    if( !dotation.type_ )
        return;
    const QString type = dotation.type_->GetName().c_str();
    Dotation& supply = availableSupplies_[ type ];
    if( !supply.type_ )
    {
        dotationTypes_.append( type );
        supply.type_ = dotation.type_;
    }
    supply.quantity_ += dotation.quantity_;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::addCarryingEquipment
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::AddCarryingEquipment( const Entity_ABC& entity )
{
    if( auto agent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
    {
        if( !agent->Retrieve< SupplyStates >() )
            return;
        auto equipments = static_cast< const Equipments* >( entity.Retrieve< Equipments_ABC >() );
        if( !equipments || !profile_.CanBeOrdered( *agent ) )
            return;
        auto it = agent->GetType().CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::AgentComposition& composition = it.NextElement();
            if( composition.IsConvoyer() )
            {
                const unsigned long id = composition.GetType().GetId();
                tools::Iterator< const Equipment& > it = equipments->CreateIterator();
                while( it.HasMoreElements() )
                {
                    const Equipment& equipment = it.NextElement();
                    if( equipment.type_.GetLogSupplyFunctionCarrying() != 0 && equipment.type_.GetId() == id )
                    {
                        carriersTypes_[ equipment.GetName() ] += equipment.available_;
                        carriersTypeNames_[ equipment.GetName() ] = &equipment.type_;
                    }
                }
            }
        }
    }
    else
    {
        auto tacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !tacticalHierarchies )
            return;
        auto it = tacticalHierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            AddCarryingEquipment( it.NextElement() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::clearCarriersData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::ClearCarriersData()
{
    carriersTypes_.clear();
    carriersTypeNames_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::OnCarriersUseCheckStateChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::OnCarriersUseCheckStateChanged()
{
    carriersTable_->setEnabled( carriersUseCheck_->isChecked() );
}

QString LogisticSupplyFlowDialog_ABC::GetErrorText( const sword::UnitMagicActionAck& ack )
{
    if( ack.has_error_code() )
        switch( ack.error_code() )
        {
        case sword::UnitActionAck::error_supply_denied:
            return tools::translate( "LogisticSupplyFlowDialog_ABC", "The request has been denied" );
        case sword::UnitActionAck::error_undeployed:
            return tools::translate( "LogisticSupplyFlowDialog_ABC", "Missing deployed suppliers" );
        }
    return tools::translate( "LogisticSupplyFlowDialog_ABC", "This request cannot be resolved" );
}

void LogisticSupplyFlowDialog_ABC::EnableButtons( bool enabled )
{
    ok_->setEnabled( enabled );
    cancel_->setEnabled( enabled );
}
