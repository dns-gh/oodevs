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
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "LogisticSupplyCarriersTableWidget.h"

#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/resources.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
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

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyFlowDialog_ABC::LogisticSupplyFlowDialog_ABC( QWidget* parent,
                                                            kernel::Controllers& controllers,
                                                            actions::ActionsModel& actionsModel,
                                                            const ::StaticModel& staticModel,
                                                            const kernel::Time_ABC& simulation,
                                                            gui::ParametersLayer& layer,
                                                            const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                                            const kernel::Profile_ABC& profile )
    : QDialog( parent, tr( "Supply flow" ), 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , automats_( automats )
    , selected_( controllers )
    , startWaypointLocation_( false )
    , layer_( layer )
    , profile_( profile )
{
    waypointLocationCreator_ = new LocationCreator( 0, layer, *this );
    waypointLocationCreator_->Allow( false, false, false, false, false );
    routeLocationCreator_ = new LocationCreator( 0, layer, *this );
    routeLocationCreator_->Allow( false, false, false, false, false );

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
    QWidget* routeTab = new QWidget( tabs_ );
    QGroupBox* upDownGroup = new QGroupBox( routeTab );
    upDownGroup->setFlat( true );
    tabs_->addTab( resourcesTab_, tr( "Supplies" ) );
    tabs_->addTab( carriersTab, tr( "Carriers" ) );
    tabs_->addTab( routeTab, tr( "Route" ) );
    cancel_ = new QPushButton( tr( "Cancel" ), tabs_ );
    ok_ = new QPushButton( tr( "Ok" ), tabs_ );
    connect( cancel_, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( ok_, SIGNAL( clicked() ), SLOT( Validate() ) );

    moveUpButton_ = new QPushButton( upDownGroup );
    moveDownButton_ = new QPushButton( upDownGroup );
    addWaypointButton_ = new QPushButton( upDownGroup );
    delWaypointButton_ = new QPushButton( upDownGroup );
    moveUpButton_->setIcon( MAKE_ICON( arrow_up ) );
    moveDownButton_->setIcon( MAKE_ICON( arrow_down ) );
    delWaypointButton_->setIcon( MAKE_ICON( trash ) );
    addWaypointButton_->setIcon( MAKE_ICON( add_point ) );
    moveUpButton_->setToolTip( tr( "Move waypoint up" ) );
    moveDownButton_->setToolTip( tr( "Move waypoint down" ) );
    delWaypointButton_->setToolTip( tr( "Delete Waypoint") );
    addWaypointButton_->setToolTip( tr( "Add Waypoint") );
    connect( moveUpButton_, SIGNAL( clicked() ), SLOT( MoveUpWaypoint() ) );
    connect( moveDownButton_, SIGNAL( clicked() ), SLOT( MoveDownWaypoint() ) );
    connect( delWaypointButton_, SIGNAL( clicked() ), SLOT( DeleteWaypoint() ) );
    connect( addWaypointButton_, SIGNAL( clicked() ), SLOT( AddWaypoint() ) );
    connect( tabs_, SIGNAL( currentChanged( int ) ), SLOT( OnTabChanged( int ) ) );

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

    waypointList_ = new QListView( routeTab );
    waypointList_->setViewMode( QListView::ListMode );
    waypointList_->setSelectionMode( QAbstractItemView::SingleSelection );
    waypointList_->setMovement( QListView::Snap );
    waypointList_->setDragDropMode( QAbstractItemView::InternalMove );
    waypointList_->setDragDropOverwriteMode( false );
    waypointList_->setModel( new CustomStringListModel( QStringList() ) );
    waypointList_->setMinimumSize( 235, 300 );
    connect( waypointList_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnWaypointSelect() ) );
    connect( waypointList_->model(), SIGNAL( modelReset() ), this, SLOT( OnWaypointRowChanged() ) );
    connect( waypointList_->model(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( OnWaypointRowChanged() ) );
    connect( waypointList_->model(), SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( OnWaypointRowChanged() ) );
    connect( waypointList_->model(), SIGNAL( rowsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SLOT( OnWaypointRowChanged() ) );
    connect( waypointList_->model(), SIGNAL( rowsRemoved ( const QModelIndex&, int, int ) ), this, SLOT( OnWaypointRowChanged() ) );
    QGridLayout* waypointLayout = new QGridLayout( routeTab, 2, 2 );
    QVBoxLayout* boxLayout = new QVBoxLayout();
    boxLayout->addWidget( moveUpButton_ );
    boxLayout->addWidget( moveDownButton_ );
    boxLayout->addWidget( delWaypointButton_ );
    boxLayout->addWidget( addWaypointButton_ );
    boxLayout->setAlignment( Qt::AlignBottom );
    upDownGroup->setLayout( boxLayout );
    waypointLayout->addWidget( waypointList_, 0, 0, 1, 1 );
    waypointLayout->addWidget( upDownGroup, 0, 1, 1, 1 );
    waypointLayout->setSpacing( 5 );
    waypointLayout->setMargin( 10 );

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
// Name: LogisticSupplyFlowDialog_ABC::AddWaypoint
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::AddWaypoint()
{
    if( !startWaypointLocation_ )
    {
        controllers_.Unregister( *routeLocationCreator_ );
        controllers_.Update( *waypointLocationCreator_ );
    }
    startWaypointLocation_ = true;
    waypointLocationCreator_->StartPoint();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::GetSelectedWaypoint
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
QString LogisticSupplyFlowDialog_ABC::GetSelectedWaypoint()
{
    QModelIndexList indexList = waypointList_->selectionModel()->selectedIndexes();
    if( indexList.count() == 0 )
        return QString();

    int row = indexList.first().row();
    return static_cast< CustomStringListModel* >( waypointList_->model() )->stringList().at( row );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::DeleteWaypoint
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::DeleteWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    T_PointNames::iterator itPoint = points_.find( waypoint );
    if( itPoint != points_.end() )
    {
        points_.erase( itPoint );
        CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        waypoints.remove( waypoint );
        pModel->setStringList( waypoints );
    }

    moveUpButton_->setEnabled( false );
    moveDownButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::MoveUpWaypoint
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::MoveUpWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    if( !waypoint.isEmpty() )
    {
        CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        int index = waypoints.indexOf( waypoint );
        if( index > 0 )
        {
            waypoints.swap( index, index-1 );
            pModel->setStringList( waypoints );
            waypointList_->selectionModel()->select( pModel->index( index-1, 0 ), QItemSelectionModel::Select );
        }
    }
    waypointList_->setFocus();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::MoveDownWaypoint
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::MoveDownWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    if( !waypoint.isEmpty() )
    {
        CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        int index = waypoints.indexOf( waypoint );
        if( index < waypoints.size()-1 )
        {
            QModelIndexList curSelection = waypointList_->selectionModel()->selectedIndexes();
            waypoints.swap( index, index+1 );
            pModel->setStringList( waypoints );
            waypointList_->selectionModel()->select( pModel->index( index+1, 0 ), QItemSelectionModel::Select );
        }
    }
    waypointList_->setFocus();
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
// Name: LogisticSupplyFlowDialog_ABC::Handle
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::Handle( Location_ABC& location )
{
    if( startWaypointLocation_ && location.IsValid() )
    {
        location.Accept( *this );
        CustomStringListModel* pModel = static_cast< CustomStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        QString locationName = static_.coordinateConverter_.ConvertToMgrs( selectedPoint_ ).c_str();
        points_[ locationName ] = selectedPoint_;
        waypoints.append( locationName );
        pModel->setStringList( waypoints );
        moveUpButton_->setEnabled( false );
        moveDownButton_->setEnabled( false );
    }
    if( startWaypointLocation_ )
    {
        controllers_.Unregister( *waypointLocationCreator_ );
        controllers_.Update( *routeLocationCreator_ );
        routeLocationCreator_->StartLine();
    }
    startWaypointLocation_ = false;
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
// Name: LogisticSupplyFlowDialog_ABC::ClearRouteData
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::ClearRouteList()
{
    static_cast< CustomStringListModel* >( waypointList_->model() )->setStringList( QStringList() );
    delWaypointButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::ClearRouteData
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::ClearRouteData()
{
    points_.clear();
    routeDrawpoints_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::VisitPoint
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::VisitPoint( const geometry::Point2f& point )
{
    selectedPoint_ = point;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::OnCarriersUseCheckStateChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::OnCarriersUseCheckStateChanged()
{
    carriersTable_->setEnabled( carriersUseCheck_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::OnWaypointRowChanged
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::OnWaypointRowChanged()
{
    UpdateRouteDrawpoints();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::OnWaypoitnSelect
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::OnWaypointSelect()
{
    QString waypoint = GetSelectedWaypoint();
    delWaypointButton_->setEnabled( points_.find( waypoint ) != points_.end() );

    if( !waypoint.isEmpty() )
    {
        QStringList waypointsList = static_cast< CustomStringListModel* >( waypointList_->model() )->stringList();
        moveUpButton_->setEnabled( waypoint != waypointsList.first() );
        moveDownButton_->setEnabled( waypoint != waypointsList.last() );
    }
    else
    {
        moveUpButton_->setEnabled( false );
        moveDownButton_->setEnabled( false );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::Draw
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::Draw( const Location_ABC& /*location*/, const geometry::Rectangle2f& /*viewport*/, const GlTools_ABC& tools ) const
{
    if( startWaypointLocation_ || routeDrawpoints_.empty() )
        return;
    for( std::size_t i = 1; i < routeDrawpoints_.size(); ++i )
    {
        glColor4f( COLOR_ORANGE );
        glLineStipple( 1, tools.StipplePattern( -1 ) );
        tools.DrawCurvedArrow( routeDrawpoints_[i-1], routeDrawpoints_[i], 0.6f );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyFlowDialog_ABC::OnTabChanged
// Created: MMC 2011-09-26
// -----------------------------------------------------------------------------
void LogisticSupplyFlowDialog_ABC::OnTabChanged( int index )
{
    addWaypointButton_->setVisible( index == 2 );
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
    return tools::translate( "LogisticSupplyFlowDialog_ABC", "This request cannot be resolved." );
}

void LogisticSupplyFlowDialog_ABC::EnableButtons( bool enabled )
{
    ok_->setEnabled( enabled );
    cancel_->setEnabled( enabled );
}
