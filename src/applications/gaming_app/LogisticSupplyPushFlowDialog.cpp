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
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Automat.h"
#include "actions/Formation.h"
#include "actions/PushFlowParameters.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "gaming/Dotation.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/StaticModel.h"
#include "gaming/SupplyStates.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Dotations_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>
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
// Name: LogisticSupplyPushFlowDialog constructor
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const tools::Resolver_ABC< Automat_ABC >& automats, const Profile_ABC& profile )
    : QDialog( parent, tr( "Push supply flow" ), 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , automats_( automats )
    , profile_( profile )
    , selected_( controllers )
    , pRecipientSelected_( 0 )
{  
    setCaption( tr( "Push supply flow" ) );

    QTabWidget* tabs = new QTabWidget( this );
    QGridLayout* tabLayout = new QGridLayout( this, 3, 2 );
    tabLayout->addWidget( tabs, 0, 0, 1, 3 );

    QWidget* resourcesTab = new QWidget( tabs );
    QWidget* carriersTab = new QWidget( tabs );
    QWidget* routeTab = new QWidget( tabs );
    tabs->addTab( resourcesTab, tools::translate( "Logistic : Push supply flow", "Resources" ) );
    tabs->addTab( carriersTab, tools::translate( "Logistic : Push supply flow", "Carriers" ) );
    tabs->addTab( routeTab, tools::translate( "Logistic : Push supply flow", "Route" ) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), tabs );
    QPushButton* okAddWaypoint = new QPushButton( tr( "Add Waypoint" ), tabs );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), tabs );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Validate() ) );
    tabLayout->addWidget( okButton, 1, 0, 1, 1 );
    tabLayout->addWidget( okAddWaypoint, 1, 1, 1, 1 );
    tabLayout->addWidget( cancelButton, 1, 2, 1, 1 );
    okAddWaypoint->setVisible( false );
    this->setFixedSize( 300, 400 );

    recipientsTable_ = new Q3Table( 0, 1, resourcesTab );
    recipientsTable_->horizontalHeader()->setLabel( 0, tools::translate( "Logistic : Push supply flow", "Recipients" ) );
    recipientsTable_->setMargin( 5 );
    recipientsTable_->setLeftMargin( 0 );
    recipientsTable_->setColumnWidth( 0 , 260 );
    recipientsTable_->setNumRows( 0 );
    connect( recipientsTable_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnRecipientValueChanged( int, int ) ) );
    connect( recipientsTable_, SIGNAL( currentChanged(int, int) ), this, SLOT( OnRecipientSelectionChanged( int, int ) ) );

    resourcesTable_ = new Q3Table( 0, 3, resourcesTab );
    resourcesTable_->horizontalHeader()->setLabel( 0, tools::translate( "Logistic : Push supply flow", "Resource" ) );
    resourcesTable_->horizontalHeader()->setLabel( 1, tools::translate( "Logistic : Push supply flow", "Available" ) );
    resourcesTable_->horizontalHeader()->setLabel( 2, tools::translate( "Logistic : Push supply flow", "Quantity" ) );
    resourcesTable_->verticalHeader()->hide();
    resourcesTable_->setMargin( 5 );
    resourcesTable_->setLeftMargin( 0 );
    resourcesTable_->setColumnWidth( 0 , 120 );
    resourcesTable_->setColumnWidth( 1 , 70 );
    resourcesTable_->setColumnWidth( 2 , 70 );
    connect( resourcesTable_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnResourcesValueChanged( int, int ) ) );
    QVBoxLayout* resourcesLayout = new QVBoxLayout( resourcesTab );
    resourcesLayout->addWidget( recipientsTable_ );
    resourcesLayout->addWidget( resourcesTable_ );
    
    carriersUseCheck_ = new QCheckBox( tools::translate( "Logistic : Push supply flow", "Manual selection of transport carriers" ), carriersTab );
    connect( carriersUseCheck_, SIGNAL( stateChanged( int ) ), this, SLOT( OnCarriersUseCheckStateChanged() ) );
    QStringList carriersTitles;
    carriersTable_ = new Q3Table( 0, 3, carriersTab );
    carriersTable_->horizontalHeader()->setLabel( 0, tools::translate( "Logistic : Push supply flow", "Type" ) );
    carriersTable_->horizontalHeader()->setLabel( 1, tools::translate( "Logistic : Push supply flow", "Available" ) );
    carriersTable_->horizontalHeader()->setLabel( 2, tools::translate( "Logistic : Push supply flow", "Quantity" ) );
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
    QStringList routePoints;
    waypointList_->setModel( new customStringListModel( routePoints ) );
    waypointList_->setFixedSize( 270, 300 );
    QVBoxLayout* waypointLayout = new QVBoxLayout( routeTab );
    waypointLayout->setSizeConstraint( QLayout::SetMaximumSize );
    waypointLayout->addWidget( waypointList_ );

    controllers_.Register( *this );
    hide();
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
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::NotifyContextMenu
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::NotifyContextMenu( const Formation_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
        InsertMenuEntry( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::InsertMenuEntry
// Created: ABR 2011-06-29
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::InsertMenuEntry( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu )
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
    
    AddRecipientItem();
    AddCarrierItem();
    
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Validate
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Validate()
{
    if( !selected_ )
        return;

    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( ( dynamic_cast< const Automat_ABC* >( static_cast< const Entity_ABC* >( selected_ ) ) ) ? "automat_log_supply_push_flow" : "formation_log_supply_push_flow" );
    UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Log Supply Push Flow" ), true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

    parameters::PushFlowParameters* pushFlowParameters = new parameters::PushFlowParameters( it.NextElement() );
    BOOST_FOREACH( const T_RecipientSupplies::value_type& recipientSupply, recipientSupplies_ )
    {
        BOOST_FOREACH( const ObjectQuantity& resource, recipientSupply.second )
        {
            const DotationType* dotationType = supplies_[ resource.objectName_ ].type_;
            assert( dotationType );
            pushFlowParameters->AddResource( *dotationType, resource.quantity_, *recipientSupply.first );
        }
    }
    BOOST_FOREACH( const ObjectQuantity& carrier, carriers_ )
        pushFlowParameters->AddTransporter( *carriersTypeNames_[ carrier.objectName_ ], carrier.quantity_ );

    action->AddParameter( *pushFlowParameters );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );

    ClearRecipientsTable();
    ClearRecipientsData();
    ClearResourcesTable();
    ClearResourcesData();
    ClearCarriersTable();
    ClearCarriersData();
    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: SBO 2006-07-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    reject();
    selected_ = 0;
    ClearRecipientsTable();
    ClearRecipientsData();
    ClearResourcesTable();
    ClearResourcesData();
    ClearCarriersTable();
    ClearCarriersData();
}

namespace
{
    struct SupplyStatesVisitor : private boost::noncopyable
                               , public kernel::ExtensionVisitor_ABC<SupplyStates>
    {
        SupplyStatesVisitor( LogisticSupplyPushFlowDialog& dlg, void (LogisticSupplyPushFlowDialog::*pFunc)(const SupplyStates&) )
                : dlg_(dlg), pFunc_ ( pFunc ) {}

        void Visit( const SupplyStates& extension )
        {
            (dlg_.*pFunc_)(extension);
        }
    private:
        LogisticSupplyPushFlowDialog& dlg_;
        void (LogisticSupplyPushFlowDialog::*pFunc_)(const SupplyStates&);

    };
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddDotation
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddDotation( const SupplyStates& states )
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
// Name: LogisticSupplyPushFlowDialog::AddRecipientItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddRecipientItem()
{
    ExclusiveComboTableItem* curItem = 0;
    int current = 0;
    int row = recipientsTable_->numRows();
    if( row > 0 )
    {
        curItem = static_cast< ExclusiveComboTableItem* >( recipientsTable_->item( row-1, 0 ) );
        current = curItem->currentItem();
    }

    QStringList recipientsList;
    ComputeAvailableRecipients( recipientsList );
    const unsigned int rows = recipientsTable_->numRows();
    recipientsTable_->setNumRows( rows + 1 );
    recipientsTable_->setItem( rows, 0, new ExclusiveComboTableItem( recipientsTable_, recipientsList ) );
    recipientsTable_->setCurrentCell( rows, 0 );

    if( curItem )
        curItem->setCurrentItem( current );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddResourceItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddResourceItem()
{
    ExclusiveComboTableItem* curItem = 0;
    int current = 0;
    int row = resourcesTable_->numRows();
    if( row > 0 )
    {
        curItem = static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( row-1, 0 ) );
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
// Name: LogisticSupplyPushFlowDialog::AddResourceItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddResourceItem( QString dotationName, int Available, int qtySupply )
{
    AddResourceItem();

    assert( resourcesTable_->numRows() > 0 );
    const int rowIndex = resourcesTable_->numRows() - 1;
    static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( rowIndex, 0 ) )->setCurrentItem( dotationName );
    resourcesTable_->item( rowIndex, 1 )->setText( QString::number( Available ) );
    resourcesTable_->item( rowIndex, 2 )->setText( QString::number( qtySupply ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddCarrierItem
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddCarrierItem()
{
    ExclusiveComboTableItem* curItem = 0;
    int current = 0;
    int row = carriersTable_->numRows();
    if( row > 0 )
    {
        curItem = static_cast< ExclusiveComboTableItem* >( carriersTable_->item( row-1, 0 ) );
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
// Name: LogisticSupplyPushFlowDialog::AddWaypoint
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddCarrierItem( QString dotationName, int Available, int qtySupply )
{
    AddCarrierItem();

    assert( carriersTable_->numRows() > 0 );
    const int rowIndex = carriersTable_->numRows() - 1;
    static_cast< ExclusiveComboTableItem* >( carriersTable_->item( rowIndex, 0 ) )->setCurrentItem( dotationName );
    carriersTable_->item( rowIndex, 1 )->setText( QString::number( Available ) );
    carriersTable_->item( rowIndex, 2 )->setText( QString::number( qtySupply ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::AddWaypoint
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddWaypoint()
{
    ;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearRecipientsTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearRecipientsTable()
{
    if( recipientsTable_->numRows() > 0 )
        for( int i=recipientsTable_->numRows()-1; i>=0; --i )
            recipientsTable_->removeRow( i );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearResourcesTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearResourcesTable()
{
    if( resourcesTable_->numRows() > 0 )
        for( int i=resourcesTable_->numRows()-1; i>=0; --i )
            resourcesTable_->removeRow( i );
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
// Name: LogisticSupplyPushFlowDialog::clearRecipientsData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearResourcesData()
{
    supplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearCarriersTable
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearCarriersTable()
{
    if( carriersTable_->numRows() > 0 )
        for( int i=carriersTable_->numRows()-1; i>=0; --i )
            carriersTable_->removeRow( i );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearCarriersData
// Created: MMC 2011-09-20
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ClearCarriersData()
{
    carriersTypes_.clear();
    carriersTypeNames_.clear();
    carriers_.clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::clearRecipientsTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::EraseRecipientData( int index )
{
    if( index >= recipients_.size() )
        return;
    T_Recipients::iterator it = recipients_.begin(); std::advance( it, index );
    T_RecipientSupplies::iterator itSupplies = recipientSupplies_.find( *it );
    if( itSupplies != recipientSupplies_.end() )
        recipientSupplies_.erase( itSupplies );
    recipients_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::addCarryingEquipment
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::AddCarryingEquipment( const kernel::Entity_ABC& entity )
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
// Name: LogisticSupplyPushFlowDialog::computeAvailableCarriers
// Created: MMC 2011-09-21
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeAvailableCarriers( QStringList& carriersNames )
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
// Name: LogisticSupplyPushFlowDialog::clearRecipientsTable
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::InsertNewRecipientData( int index, const kernel::Automat_ABC* pRecipient )
{
    T_SuppliesVector emptySupplies;
    if( recipientSupplies_.find( pRecipient ) == recipientSupplies_.end() )
        recipientSupplies_[ pRecipient ] = emptySupplies;
    recipients_.insert( recipients_.begin() + index, pRecipient );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnRecipientValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnRecipientValueChanged( int row, int /*col*/ )
{
    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( recipientsTable_->item( row, 0 ) );
    QString selection = item.currentText();
    if( selection.isEmpty() )
    {
        if( recipientsTable_->numRows() > 1 )
            recipientsTable_->removeRow( row );
        EraseRecipientData( row );
    }
    else
    {
        EraseRecipientData( row );
        const kernel::Automat_ABC* pRecipient = recipientsNames_[ selection ];
        if( pRecipient )
            InsertNewRecipientData( row, pRecipient );
        if( row + 1 == recipientsTable_->numRows() )      
            AddRecipientItem();
    }

    OnRecipientSelectionChanged( row , 0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnRecipientValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnRecipientSelectionChanged( int row, int /*col*/ )
{
    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( recipientsTable_->item( row, 0 ) );
    QString selection = item.currentText();
    if( selection.isEmpty() )
        pRecipientSelected_ = 0;
    else
        pRecipientSelected_ = recipientsNames_[ selection ];

    if( !pRecipientSelected_ )
    {
        supplies_.clear();
        ClearResourcesTable();
    }
    else
    {
        T_RecipientSupplies::iterator it = recipientSupplies_.find( pRecipientSelected_ );
        if( it != recipientSupplies_.end() )
        {
            supplies_.clear();
            ClearResourcesTable();

            SupplyStatesVisitor visitor( *this, &LogisticSupplyPushFlowDialog::AddDotation );
            selected_->Get< kernel::TacticalHierarchies >().Accept< SupplyStates >( visitor );

            T_SuppliesVector& supplies = it->second;
            for( int i=0; i < supplies.size(); ++i )
            {
                const QString& dotationName = supplies[i].objectName_;
                int available = 0;
                T_Supplies::iterator itDotation = supplies_.find( dotationName );
                if( itDotation != supplies_.end() )
                    available = itDotation->second.quantity_;
                AddResourceItem( dotationName, available, supplies[i].quantity_ );
            }

            AddResourceItem();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::computeAvailableRecipients
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::ComputeAvailableRecipients( QStringList& recipientsNames )
{
    recipientsNames_.clear();
    recipientsNames.clear(); 
    recipientsNames.append( QString() );
    
    tools::Iterator< const Automat_ABC& > it = automats_.CreateIterator();
    const kernel::Entity_ABC& team = selected_->Get< kernel::TacticalHierarchies >().GetTop();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        if( (const Entity_ABC*)&automat != selected_ )
        {
            const kernel::AutomatType& type = automat.GetType();
            if( type.IsLogisticSupply() && &automat.Get< kernel::TacticalHierarchies >().GetTop() == &team )
            {
                recipientsNames_[ automat.GetName() ] = &automat;
                recipientsNames.append( automat.GetName() );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnResourcesValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnResourcesValueChanged( int row, int col )
{
    if( !pRecipientSelected_ )
        return;

    T_RecipientSupplies::iterator it = recipientSupplies_.find( pRecipientSelected_ );
    if( it == recipientSupplies_.end() )
        return;

    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( resourcesTable_->item( row, 0 ) );
    QString selection = item.currentText();
    Q3TableItem& itemAVailable = *resourcesTable_->item( row, 1 );
    Q3TableItem& itemValue = *resourcesTable_->item( row, 2 );
    int newValue = itemValue.text().toInt();

    const Dotation& dotationSelected = supplies_[ selection ];
    T_SuppliesVector& supplies = it->second;
    if( row == supplies.size() )
    {
        ObjectQuantity newDotation( selection, 1 );
        supplies.push_back( newDotation );
    }

    T_SuppliesVector::iterator itSupplies = supplies.begin(); std::advance( itSupplies, row );
    if( selection.isEmpty() )
    {
        if( resourcesTable_->numRows() > 1 )
            resourcesTable_->removeRow( row );
        supplies.erase( itSupplies );
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
void LogisticSupplyPushFlowDialog::OnCarriersUseCheckStateChanged()
{
    carriersTable_->setEnabled( carriersUseCheck_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnCarriersValueChanged
// Created: MMC 2011-09-19
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnCarriersValueChanged( int row, int col )
{
    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( carriersTable_->item( row, 0 ) );
    QString selection = item.currentText();
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