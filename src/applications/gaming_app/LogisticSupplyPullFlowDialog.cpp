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
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "actions/PullFlowParameters.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_gui/resources.h"
#include "protocol/SimulationSenders.h"
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;
using namespace parameters;

namespace 
{
    class customStringListModel : public QStringListModel
    {
    public:
        customStringListModel( const QStringList& strings, QObject* parent = 0 ) : QStringListModel( strings, parent ) {}
        Qt::ItemFlags flags( const QModelIndex& index ) const
        {
            Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
            if( !index.isValid() )
                flags |= Qt::ItemIsDropEnabled;
            return flags;
        }
    };
};

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog constructor
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
LogisticSupplyPullFlowDialog::LogisticSupplyPullFlowDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel,
        const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const tools::Resolver_ABC< Automat_ABC >& automats,
        const tools::Resolver_ABC< kernel::Formation_ABC >& formations, const Profile_ABC& profile )
    : QDialog( parent, tr( "Pull supply flow" ) )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , automats_( automats )
    , formations_( formations )
    , profile_( profile )
    , selected_( controllers )
    , supplier_( 0 )
    , startWaypointLocation_( false )
    , layer_( layer )
{
    setCaption( tr( "Pull supply flow" ) );

    waypointLocationCreator_ = new LocationCreator( 0, layer, *this );
    waypointLocationCreator_->Allow( false, false, false, false, false );
    routeLocationCreator_ = new LocationCreator( 0, layer, *this );
    routeLocationCreator_->Allow( false, false, false, false, false );

    tabs_ = new QTabWidget( this );
    tabs_->setMargin( 5 );
    QGridLayout* tabLayout = new QGridLayout( this, 1, 2 );
    tabLayout->addWidget( tabs_, 0, 0, 1, 3 );

    QWidget* resourcesTab = new QWidget( tabs_ );
    QWidget* carriersTab = new QWidget( tabs_ );
    QWidget* routeTab = new QWidget( tabs_ );
    QGroupBox* upDownGroup = new QGroupBox( routeTab );
    upDownGroup->setFlat( true );
    tabs_->addTab( resourcesTab, tools::translate( "Logistic : Pull supply flow", "Resources" ) );
    tabs_->addTab( carriersTab, tools::translate( "Logistic : Pull supply flow", "Carriers" ) );
    tabs_->addTab( routeTab, tools::translate( "Logistic : Pull supply flow", "Route" ) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), tabs_ );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), tabs_ );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    moveUpButton_ = new QPushButton( upDownGroup );
    moveDownButton_ = new QPushButton( upDownGroup );
    addWaypointButton_ = new QPushButton( upDownGroup );
    delWaypointButton_ = new QPushButton( upDownGroup );       
    moveUpButton_->setIcon( MAKE_ICON( arrow_up ) );
    moveDownButton_->setIcon( MAKE_ICON( arrow_down ) );
    delWaypointButton_->setIcon( MAKE_ICON( trash ) );
    addWaypointButton_->setIcon( MAKE_ICON( add_point ) );
    moveUpButton_->setToolTip( tools::translate( "Logistic : Push supply flow", "Move waypoint up" ) );
    moveDownButton_->setToolTip( tools::translate( "Logistic : Push supply flow", "Move waypoint down" ) );
    delWaypointButton_->setToolTip( tools::translate( "Logistic : Push supply flow", "Delete Waypoint") );
    addWaypointButton_->setToolTip( tools::translate( "Logistic : Push supply flow", "Add Waypoint") );
    connect( moveUpButton_, SIGNAL( clicked() ), SLOT( MoveUpWaypoint() ) );
    connect( moveDownButton_, SIGNAL( clicked() ), SLOT( MoveDownWaypoint() ) );
    connect( delWaypointButton_, SIGNAL( clicked() ), SLOT( DeleteWaypoint() ) );
    connect( addWaypointButton_, SIGNAL( clicked() ), SLOT( AddWaypoint() ) );
    connect( tabs_, SIGNAL( currentChanged( int ) ), SLOT( OnTabChanged( int ) ) );


    tabLayout->addWidget( okButton, 1, 0, 1, 1 );
    tabLayout->addWidget( cancelButton, 1, 2, 1, 1 );
    this->setFixedSize( 340, 420 );
    tabLayout->setMargin( 5 );
    tabLayout->setSpacing( 5 );

    supplierCombo_ = new ValuedComboBox< const Entity_ABC* >( resourcesTab );
    supplierCombo_->setMinimumWidth( 260 );
    connect( supplierCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSupplierValueChanged() ) );

    resourcesTable_ = new Q3Table( 0, 3, resourcesTab );
    resourcesTable_->horizontalHeader()->setLabel( 0, tools::translate( "Logistic : Pull supply flow", "Resource" ) );
    resourcesTable_->horizontalHeader()->setLabel( 1, tools::translate( "Logistic : Pull supply flow", "Available" ) );
    resourcesTable_->horizontalHeader()->setLabel( 2, tools::translate( "Logistic : Pull supply flow", "Quantity" ) );
    resourcesTable_->verticalHeader()->hide();
    resourcesTable_->setMargin( 5 );
    resourcesTable_->setLeftMargin( 0 );
    resourcesTable_->setColumnWidth( 0 , 120 );
    resourcesTable_->setColumnWidth( 1 , 70 );
    resourcesTable_->setColumnWidth( 2 , 70 );
    connect( resourcesTable_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnResourcesValueChanged( int, int ) ) );
    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab );
    resourcesLayout->addWidget( new QLabel( tools::translate( "Logistic : Pull supply flow", "Supplier:" ), resourcesTable_ ) );
    resourcesLayout->addWidget( supplierCombo_ );
    resourcesLayout->addWidget( resourcesTable_ );

    carriersUseCheck_ = new QCheckBox( tools::translate( "Logistic : Pull supply flow", "Manual selection of transport carriers" ), carriersTab );
    connect( carriersUseCheck_, SIGNAL( stateChanged( int ) ), this, SLOT( OnCarriersUseCheckStateChanged() ) );
    QStringList carriersTitles;
    carriersTable_ = new Q3Table( 0, 3, carriersTab );
    carriersTable_->horizontalHeader()->setLabel( 0, tools::translate( "Logistic : Pull supply flow", "Type" ) );
    carriersTable_->horizontalHeader()->setLabel( 1, tools::translate( "Logistic : Pull supply flow", "Available" ) );
    carriersTable_->horizontalHeader()->setLabel( 2, tools::translate( "Logistic : Pull supply flow", "Quantity" ) );
    carriersTable_->setEnabled( false );
    carriersTable_->setMargin( 5 );
    carriersTable_->setLeftMargin( 0 );
    carriersTable_->setColumnWidth( 0 , 120 );
    carriersTable_->setColumnWidth( 1 , 70 );
    carriersTable_->setColumnWidth( 2 , 70 );
    connect( carriersTable_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnCarriersValueChanged( int, int ) ) );
    QVBoxLayout* carriersLayout = new QVBoxLayout( carriersTab );
    carriersLayout->addWidget( carriersUseCheck_ );
    carriersLayout->addWidget( carriersTable_ );

    waypointList_ = new QListView( routeTab );
    waypointList_->setViewMode( QListView::ListMode );
    waypointList_->setSelectionMode( QAbstractItemView::SingleSelection );
    waypointList_->setMovement( QListView::Snap );
    waypointList_->setDragDropMode( QAbstractItemView::InternalMove );
    waypointList_->setDragDropOverwriteMode( false );
    waypointList_->setModel( new customStringListModel( QStringList() ) );
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

    controllers_.Register( *this );
    hide();
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
        if( agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
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

    AddCarrierItem();
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

namespace
{
    std::string CreateName( const std::string& str, int& index )
    {
        return QString( (str + " %1" ).c_str() ).arg( index++ ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Validate
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Validate()
{
    if( !selected_ || !supplier_ )
        return;

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

    parameters::PullFlowParameters* pullFlowParameters = new parameters::PullFlowParameters( it.NextElement(), static_.coordinateConverter_ );
    const kernel::Automat_ABC* pSupplierAutomat = dynamic_cast< const kernel::Automat_ABC* >( supplier_ );
    const kernel::Formation_ABC* pSupplierFormation = dynamic_cast< const kernel::Formation_ABC* >( supplier_ );
    assert( pSupplierAutomat || pSupplierFormation );
    if( pSupplierAutomat )
        pullFlowParameters->SetSupplier( *pSupplierAutomat );
    else if( pSupplierFormation )
        pullFlowParameters->SetSupplier( *pSupplierFormation );

    for( T_SuppliesVector::iterator it = supplierSupplies_.begin(); it != supplierSupplies_.end(); ++it )
    {
        const ObjectQuantity& supply = *it;
        const DotationType* dotationType = supplies_[ supply.objectName_ ].type_;
        assert( dotationType );
        pullFlowParameters->AddResource( *dotationType, supply.quantity_ );
    }

    BOOST_FOREACH( const ObjectQuantity& carrier, carriers_ )
        pullFlowParameters->AddTransporter( *carriersTypeNames_[ carrier.objectName_ ], carrier.quantity_ );

    // Route
    customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
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
    ClearResourcesTable();
    ClearResourcesData();
    ClearCarriersTable();
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
    ClearResourcesTable();
    ClearResourcesData();
    ClearCarriersTable();
    ClearCarriersData();
    ClearRouteList();
    ClearRouteData();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::closeEvent
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

namespace
{
    struct SupplyStatesVisitor : kernel::ExtensionVisitor_ABC<SupplyStates>, boost::noncopyable
    {
        SupplyStatesVisitor( LogisticSupplyPullFlowDialog& dlg, void (LogisticSupplyPullFlowDialog::*pFunc)(const SupplyStates&) )
                : dlg_(dlg), pFunc_ ( pFunc ) {}

        void Visit( const SupplyStates& extension )
        {
            (dlg_.*pFunc_)(extension);
        }
    private:
        LogisticSupplyPullFlowDialog& dlg_;
        void (LogisticSupplyPullFlowDialog::*pFunc_)(const SupplyStates&);

    };
}


// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddDotation
// Created : AHC 2010-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddDotation( const SupplyStates& states )
{
    tools::Iterator< const Dotation& > it = states.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        const QString type = dotation.type_->GetName().c_str();
        Dotation& supply = supplies_[ type ];
        if( ! supply.type_ )
        {
            dotationTypes_.append( type );
            supply.type_ = dotation.type_;
        }
        supply.quantity_ += dotation.quantity_;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddResourceItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddResourceItem()
{
    ExclusiveComboTableItem* curItem = 0;
    int current = 0;
    int row = resourcesTable_->numRows();
    if( row > 0 )
    {
        curItem = static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( row-1, 0 ) );
        if( curItem )
            current = curItem->currentItem();
    }

    QStringList resourcesList; resourcesList.append( QString() );
    for( T_Supplies::iterator itSupplies = supplies_.begin(); itSupplies != supplies_.end(); ++itSupplies )
    {
        const Dotation& dotation = itSupplies->second;
        if( dotation.type_ )
            resourcesList.append( QString( dotation.type_->GetName().c_str() ) );
    }

    const unsigned int rows = resourcesTable_->numRows();
    resourcesTable_->setNumRows( rows + 1 );
    resourcesTable_->setItem( rows, 0, new ExclusiveComboTableItem( resourcesTable_, resourcesList ) );
    resourcesTable_->setItem( rows, 1, new Q3TableItem( resourcesTable_, Q3TableItem::Never, QString() ) );
    resourcesTable_->setItem( rows, 2, new Q3TableItem( resourcesTable_, Q3TableItem::WhenCurrent, QString() ) );
    resourcesTable_->setCurrentCell( rows, 0 );

    if( curItem )
        curItem->setCurrentItem( current );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddResourceItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddResourceItem( QString dotationName, int Available, int qtySupply )
{
    AddResourceItem();

    assert( resourcesTable_->numRows() > 0 );
    const int rowIndex = resourcesTable_->numRows() - 1;
    ExclusiveComboTableItem* item = static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( rowIndex, 0 ) );
    if( item )
        item->setCurrentItem( dotationName );
    resourcesTable_->item( rowIndex, 1 )->setText( QString::number( Available ) );
    resourcesTable_->item( rowIndex, 2 )->setText( QString::number( qtySupply ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddCarrierItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddCarrierItem()
{
    ExclusiveComboTableItem* curItem = 0;
    int current = 0;
    int row = carriersTable_->numRows();
    if( row > 0 )
    {
        curItem = static_cast< ExclusiveComboTableItem* >( carriersTable_->item( row-1, 0 ) );
        if( curItem )
            current = curItem->currentItem();
    }

    QStringList carriersList;
    ComputeAvailableCarriers( carriersList );

    const unsigned int rows = carriersTable_->numRows();
    carriersTable_->setNumRows( rows + 1 );
    carriersTable_->setItem( rows, 0, new ExclusiveComboTableItem( carriersTable_, carriersList ) );
    carriersTable_->setItem( rows, 1, new Q3TableItem( carriersTable_, Q3TableItem::Never, QString() ) );
    carriersTable_->setItem( rows, 2, new Q3TableItem( carriersTable_, Q3TableItem::WhenCurrent, QString() ) );
    carriersTable_->setCurrentCell( rows, 0 );

    if( curItem )
        curItem->setCurrentItem( current );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddCarrierItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddCarrierItem( QString dotationName, int Available, int qtySupply )
{
    AddCarrierItem();

    assert( carriersTable_->numRows() > 0 );
    const int rowIndex = carriersTable_->numRows() - 1;
    ExclusiveComboTableItem* item = static_cast< ExclusiveComboTableItem* >( carriersTable_->item( rowIndex, 0 ) );
    if( item )
        item->setCurrentItem( dotationName );
    carriersTable_->item( rowIndex, 1 )->setText( QString::number( Available ) );
    carriersTable_->item( rowIndex, 2 )->setText( QString::number( qtySupply ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::AddWaypoint
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddWaypoint()
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
// Name: LogisticSupplyPullFlowDialog::GetSelectedWaypoint
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
QString LogisticSupplyPullFlowDialog::GetSelectedWaypoint()
{
    QModelIndexList indexList = waypointList_->selectionModel()->selectedIndexes();
    if( indexList.count() == 0 )
        return QString();

    int row = indexList.first().row();
    return static_cast< customStringListModel* >( waypointList_->model() )->stringList().at( row );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::DeleteWaypoint
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::DeleteWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    T_PointNames::iterator itPoint = points_.find( waypoint );
    if( itPoint != points_.end() )
    {
        points_.erase( itPoint );
        customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        waypoints.remove( waypoint );
        pModel->setStringList( waypoints );
    }

    moveUpButton_->setEnabled( false );
    moveDownButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::DeleteWaypoint
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::MoveUpWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    if ( !waypoint.isEmpty() )
    {
        customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        int index = waypoints.indexOf( waypoint );
        if ( index > 0 )
        {
            waypoints.swap( index, index-1 );
            pModel->setStringList( waypoints );
            waypointList_->selectionModel()->select( pModel->index( index-1, 0 ), QItemSelectionModel::Select );            
        }
    }
    waypointList_->setFocus();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::DeleteWaypoint
// Created: MMC 2011-09-28
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::MoveDownWaypoint()
{
    QString waypoint = GetSelectedWaypoint();
    if ( !waypoint.isEmpty() )
    {
        customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        int index = waypoints.indexOf( waypoint );
        if ( index < waypoints.size()-1 )
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
// Name: LogisticSupplyPullFlowDialog::clearSuppliersTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearSuppliersTable()
{
    supplierCombo_->setCurrentText( QString() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearResourcesTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearResourcesTable()
{
    if( resourcesTable_->numRows() > 0 )
        for( int i=resourcesTable_->numRows()-1; i>=0; --i )
            resourcesTable_->removeRow( i );
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
// Name: LogisticSupplyPullFlowDialog::clearSuppliersData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearResourcesData()
{
    supplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearCarriersTable
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearCarriersTable()
{
    if( carriersTable_->numRows() > 0 )
        for( int i=carriersTable_->numRows()-1; i>=0; --i )
            carriersTable_->removeRow( i );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearCarriersData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearCarriersData()
{
    carriersTypes_.clear();
    carriersTypeNames_.clear();
    carriers_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ClearRouteData
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearRouteList()
{
    static_cast< customStringListModel* >( waypointList_->model() )->setStringList( QStringList() );
    delWaypointButton_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ClearRouteData
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ClearRouteData()
{
    points_.clear();
    routeDrawpoints_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::clearSuppliersTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::EraseSupplierData()
{
    supplier_ = 0;
    supplierSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::addCarryingEquipment
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::AddCarryingEquipment( const kernel::Entity_ABC& entity )
{
    if( const Equipments* equipments = entity.Retrieve< Equipments >() )
    {
        tools::Iterator< const Equipment& > it = equipments->CreateIterator();
        QStringList equipmentList;
        while( it.HasMoreElements() )
        {
            const Equipment& equipment = it.NextElement();
            if( equipment.type_.IsLogSupplyFunctionCarrying() )
            {
                T_CarriersQty::iterator itEquip = carriersTypes_.find( equipment.GetName() );
                if( itEquip == carriersTypes_.end() )
                    carriersTypes_[ equipment.GetName() ] = equipment.available_;
                else
                    itEquip->second += equipment.available_;

                carriersTypeNames_[ equipment.GetName() ] = &equipment.type_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::computeAvailableCarriers
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeAvailableCarriers( QStringList& carriersNames )
{
    carriersTypes_.clear();
    carriersNames.clear();
    carriersNames.append( QString() );
    carriersTypeNames_.clear();

    const kernel::TacticalHierarchies* pTacticalHierarchies = selected_->Retrieve< kernel::TacticalHierarchies >();
    if( !pTacticalHierarchies )
        return;

    AddCarryingEquipment( *selected_ );
    tools::Iterator< const kernel::Entity_ABC& > itEnt = pTacticalHierarchies->CreateSubordinateIterator();
    while( itEnt.HasMoreElements() )
        AddCarryingEquipment( itEnt.NextElement() );

    for( T_CarriersQty::iterator it = carriersTypes_.begin(); it != carriersTypes_.end(); ++it  )
        carriersNames.append( it->first );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSupplierValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnSupplierValueChanged()
{
    customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
    QStringList waypoints = pModel->stringList();
    if( supplier_ )
        waypoints.removeAll( supplier_->GetName() );    
    EraseSupplierData();

    QString selection = supplierCombo_->text( supplierCombo_->currentIndex() );
    supplier_ = suppliersNames_[ selection ];
    if( supplier_ )
        waypoints.append( supplier_->GetName() );

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
        supplies_.clear();
        ClearResourcesTable();
    }
    else
    {
        supplies_.clear();
        ClearResourcesTable();

        SupplyStatesVisitor visitor( *this, &LogisticSupplyPullFlowDialog::AddDotation );
        selected_->Get< kernel::TacticalHierarchies >().Accept< SupplyStates >( visitor );

        for( int i=0; i < supplierSupplies_.size(); ++i )
        {
            const QString& dotationName = supplierSupplies_[i].objectName_;
            int available = 0;
            T_Supplies::iterator itDotation = supplies_.find( dotationName );
            if( itDotation != supplies_.end() )
                available = itDotation->second.quantity_;
            AddResourceItem( dotationName, available, supplierSupplies_[i].quantity_ );
        }

        AddResourceItem();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeAvailableSuppliers()
{
    suppliersNames_.clear();

    const kernel::Entity_ABC& team = selected_->Get< kernel::TacticalHierarchies >().GetTop();
    {
        tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Automat_ABC& automat = it.NextElement();
            if( &automat != selected_ )
            {
                if( automat.GetLogisticLevel()!=kernel::LogisticLevel::none_ && &automat.Get< kernel::TacticalHierarchies >().GetTop() == &team )
                    suppliersNames_[ automat.GetName() ] = &automat;
            }
        }
    }
    {
        tools::Iterator< const Formation_ABC& > it = formations_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const Formation_ABC& formation = it.NextElement();
            if( formation.GetLogisticLevel()!=kernel::LogisticLevel::none_ && &formation.Get< kernel::TacticalHierarchies >().GetTop() == &team )
                suppliersNames_[ formation.GetName() ] = &formation;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnResourcesValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnResourcesValueChanged( int row, int col )
{
    if( !supplier_ )
        return;

    ExclusiveComboTableItem* item = static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( row, 0 ) );
    QString selection;
    if( item )
        selection = item->currentText();
    if( selection.isEmpty() && ( row + 1 == resourcesTable_->numRows() ) )
        return;

    Q3TableItem& itemAVailable = *resourcesTable_->item( row, 1 );
    Q3TableItem& itemValue = *resourcesTable_->item( row, 2 );
    int newValue = itemValue.text().toInt();

    const Dotation& dotationSelected = supplies_[ selection ];
    if( row == supplierSupplies_.size() )
    {
        ObjectQuantity newDotation( selection, 1 );
        supplierSupplies_.push_back( newDotation );
    }

    T_SuppliesVector::iterator itSupplies = supplierSupplies_.begin(); std::advance( itSupplies, row );
    if( selection.isEmpty() )
    {
        if( resourcesTable_->numRows() > 1 )
            resourcesTable_->removeRow( row );
        supplierSupplies_.erase( itSupplies );       
    }
    else
    {
        if( row + 1 == resourcesTable_->numRows() )
        {
            if( col == 0 )
            {
                itemValue.setText( QString("1") );
                itemAVailable.setText( QString::number( dotationSelected.quantity_ ) );
            }
            AddResourceItem();
        }
        else
        {   
            ObjectQuantity& dotationQty = *itSupplies;
            if( col == 0 )
            {
                dotationQty.objectName_ = selection;
                dotationQty.quantity_ = 1;
                itemValue.setText( QString("1") );
                itemAVailable.setText( QString::number( dotationSelected.quantity_ ) );
            }
            else if( col == 2 )
                dotationQty.quantity_ = newValue;
        }
    }

    resourcesTable_->updateCell( row, 1 );
    resourcesTable_->updateCell( row, 2 );
    resourcesTable_->updateCell( row, 3 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnCarriersUseCheckStateChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnCarriersUseCheckStateChanged()
{
    carriersTable_->setEnabled( carriersUseCheck_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnCarriersValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnCarriersValueChanged( int row, int col )
{
    ExclusiveComboTableItem* item = static_cast< ExclusiveComboTableItem* >( carriersTable_->item( row, 0 ) );
    QString selection;
    if( item )
        selection = item->currentText();
    Q3TableItem& itemAVailable = *carriersTable_->item( row, 1 );
    Q3TableItem& itemValue = *carriersTable_->item( row, 2 );
    int newValue = itemValue.text().toInt();

    unsigned int equipementSelectedAvailable = carriersTypes_[ selection ];
    if( row == carriers_.size() )
    {
        ObjectQuantity newEquipment( selection, 1 );
        carriers_.push_back( newEquipment );
    }

    T_SuppliesVector::iterator itSupplies = carriers_.begin(); std::advance( itSupplies, row );
    if( selection.isEmpty() )
    {
        if( carriersTable_->numRows() > 1 )
            carriersTable_->removeRow( row );
        carriers_.erase( itSupplies );
    }
    else
    {
        if( row + 1 == carriersTable_->numRows() )
        {
            if( col == 0 )
            {
                itemValue.setText( QString("1") );
                itemAVailable.setText( QString::number( equipementSelectedAvailable ) );
            }
            AddCarrierItem();
        }
        else
        {   
            ObjectQuantity& equipementQty = *itSupplies;
            if( col == 0 )
            {
                equipementQty.objectName_ = selection;
                equipementQty.quantity_ = 1;
                itemValue.setText( QString("1") );
                itemAVailable.setText( QString::number( equipementSelectedAvailable ) );
            }
            else if( col == 2 )
                equipementQty.quantity_ = newValue;
        }
    }

    carriersTable_->updateCell( row, 1 );
    carriersTable_->updateCell( row, 2 );
    carriersTable_->updateCell( row, 3 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::OnWaypoitnSelect
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnWaypointSelect()
{
    QString waypoint = GetSelectedWaypoint();
    delWaypointButton_->setEnabled( points_.find( waypoint ) != points_.end() );

    if ( !waypoint.isEmpty() )
    {
        QStringList waypointsList = static_cast< customStringListModel* >( waypointList_->model() )->stringList();        
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
// Name: LogisticSupplyPullFlowDialog::OnWaypointRowChanged
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnWaypointRowChanged()
{
    UpdateRouteDrawpoints();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Handle
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Handle( kernel::Location_ABC& location )
{
    if( startWaypointLocation_ && location.IsValid() )
    {
        location.Accept( *this );
        customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
        QStringList waypoints = pModel->stringList();
        QString locationName = QString( static_cast< CoordinateConverter& >( static_.coordinateConverter_ ).ConvertToMgrs( selectedPoint_ ).c_str() );
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
// Name: LogisticSupplyPullFlowDialog::VisitPoint
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::VisitPoint( const geometry::Point2f& point )
{
    selectedPoint_ = point;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::ComputeRoute
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::ComputeRoute( T_Route& route )
{
    route.clear();
    customStringListModel* pModel = static_cast< customStringListModel* >( waypointList_->model() );
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

    const kernel::Positions* startPos = static_cast< const kernel::Positions* >( selected_->Retrieve< kernel::Positions >() );
    if( startPos )
        routeDrawpoints_.push_back( startPos->GetPosition() );

    for( int i=0; i < route.size(); ++i )
        if( route[i].isPoint() )
            routeDrawpoints_.push_back( route[i].point_ );
        else
        {
            const kernel::Positions* pos = static_cast< const kernel::Positions* >( supplier_->Retrieve< kernel::Positions >() );
            if( pos )
                routeDrawpoints_.push_back( pos->GetPosition() );
        }
    
    if( startPos )
        routeDrawpoints_.push_back( startPos->GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::Draw
// Created: MMC 2011-09-22
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::Draw( const kernel::Location_ABC& /*location*/, const geometry::Rectangle2f& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    if( startWaypointLocation_ || routeDrawpoints_.empty() )
        return;

    for( int i=1; i < routeDrawpoints_.size(); ++i )
    {
        glColor4f( COLOR_ORANGE );
        glLineStipple( 1, tools.StipplePattern( -1 ) );
        tools.DrawCurvedArrow( routeDrawpoints_[i-1], routeDrawpoints_[i], 0.6f );
    }
}   

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPullFlowDialog::OnTabChanged
// Created: MMC 2011-09-26
// -----------------------------------------------------------------------------
void LogisticSupplyPullFlowDialog::OnTabChanged( int index )
{
    addWaypointButton_->setVisible( index == 2 );
}