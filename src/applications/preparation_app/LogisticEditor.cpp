// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticEditor.h"
#include "moc_LogisticEditor.cpp"

#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/tools.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticEditor::LogisticEditor( QWidget* parent, const QString& objectName, Controllers& controllers, const ::StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , staticModel_( staticModel )
    , controllers_( controllers )
    , selected_   ( controllers )
{
    setCaption( tools::translate( "StocksEditionDialog", "Stocks Edition" ) );
    gui::SubObjectName subObject( objectName );
    resize( 550, 350 );

    dataModel_ = new QStandardItemModel( this );

    delegate_ = new gui::CommonDelegate( this );
    delegate_->AddDoubleSpinBoxOnColumn( eDays, 0, std::numeric_limits< double >::max() );

    tableView_ = new gui::RichWidget< QTableView >( "tableView", this );
    tableView_->setModel( dataModel_ );
    tableView_->setItemDelegate( delegate_ );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->verticalHeader()->setVisible( false );

    validateButton_ = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    cancelButton_   = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( validateButton_ );
    buttonLayout->addWidget( cancelButton_ );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( tableView_ );
    layout->addLayout( buttonLayout );

    connect( dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnValueChanged( QStandardItem* ) ) );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton_, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor destructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticEditor::~LogisticEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::NotifyUpdated
// Created: JSR 2011-10-03
// -----------------------------------------------------------------------------
void LogisticEditor::NotifyUpdated( const ModelLoaded& )
{
    dataModel_->clear();
    QStringList horizontalHeaders;
    dataModel_->setColumnCount( 2 );
    horizontalHeaders << tr( "Category" ) <<  tr( "Days" );
    dataModel_->setHorizontalHeaderLabels( horizontalHeaders );
    tableView_->horizontalHeader()->setResizeMode( eCategory, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eDays, QHeaderView::Stretch );
    auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
    unsigned int row = 0;
    QStandardItem* item = 0;
    while( itLogClass.HasMoreElements() )
    {
        const kernel::LogisticSupplyClass& logClass = itLogClass.NextElement();
        item = new QStandardItem();
        item->setEditable( false );
        item->setCheckable( true );
        item->setCheckState( Qt::Checked );
        item->setText( logClass.GetName().c_str() );
        dataModel_->setItem( row, eCategory, item );

        const double defaultValue = 1.;
        item = new QStandardItem();
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
        item->setText( locale().toString( defaultValue, 'f', 2 ) );
        dataModel_->setItem( row, eDays, item );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::NotifyUpdated
// Created: JSR 2011-10-03
// -----------------------------------------------------------------------------
void LogisticEditor::NotifyUpdated( const ModelUnLoaded& )
{
    dataModel_->clear();
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::OnValueChanged
// Created: JSR 2011-10-04
// -----------------------------------------------------------------------------
void LogisticEditor::OnValueChanged( QStandardItem* item )
{
    if( item && item->column() == eCategory )
        if( QStandardItem* day = dataModel_->item( item->row(), eDays ) )
            day->setEnabled( item->checkState() == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::Validate
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::Validate()
{
    SupplyHierarchy( selected_ );
    Accept();
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::Accept
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::Accept()
{
    accept();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::Reject
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::closeEvent
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::NotifyContextMenu
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::NotifyContextMenu( const Automat_ABC& automat, ContextMenu& menu )
{
    if( automat.GetId() == 0 || !automat.Get< gui::LogisticBase >().IsBase() )
        return;
    selected_ = static_cast< const Entity_ABC* >( &automat );
    Update( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::NotifyContextMenu
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    if( formation.GetId() == 0 || !formation.Get< gui::LogisticBase >().IsBase() )
        return;
    selected_ = static_cast< const Entity_ABC* >( &formation );
    Update( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::EvaluateOneDaySupplyQty
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticEditor::SupplyHierarchy( SafePointer< Entity_ABC > entity )
{
    const gui::LogisticHierarchiesBase* pLogHierarchy = entity->Retrieve< gui::LogisticHierarchiesBase >();
    if( !pLogHierarchy )
        return;
    if( !logistic_helpers::IsLogisticBase( *entity ) )
        return;
    SupplyHierarchy( *entity, *pLogHierarchy );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::FillSupplyRequirements
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticEditor::FillSupplyRequirements( const Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    const Agent_ABC* pAgent = dynamic_cast< const Agent_ABC* >( &entity );
    if( pAgent )
        ComputeRequirements( *pAgent, logType, requirements );
    const TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< TacticalHierarchies >();
    if( pTacticalHierarchies )
    {
        auto children = pTacticalHierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Entity_ABC& childrenEntity = children.NextElement();
            FillSupplyRequirements( childrenEntity, logType, requirements );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::SupplyLogisticBaseStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticEditor::SupplyLogisticBaseStocks( const Entity_ABC& blLogBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    auto logChildren = blLogBase.Get< gui::LogisticHierarchiesBase >().CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const Entity_ABC& entity = logChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( entity ) )
        {
            if( blLogBase.GetId() != entity.GetId() )
                SupplyLogisticBaseStocks( entity, logType, requirements );
        }
        else
            FillSupplyRequirements( entity, logType, requirements );
    }
    const Automat_ABC* pTC2 = dynamic_cast< const Automat_ABC* >( &blLogBase );
    if( pTC2 && pTC2->Get< gui::LogisticBase >().IsBase() )
        FillSupplyRequirements( blLogBase, logType, requirements );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticEditor::ComputeRequirements( const Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    AgentType& agentType = staticModel_.types_.tools::Resolver< AgentType >::Get( agent.GetType().GetId() );
    auto agentCompositionIterator = agentType.CreateIterator();
    while( agentCompositionIterator.HasMoreElements() )
    {
        const AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< EquipmentType >::Get( agentComposition.GetType().GetId() );
        auto resourcesIterator = equipmentType.CreateResourcesIterator();
        while( resourcesIterator.HasMoreElements() )
        {
            const DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
            const DotationType& category = staticModel_.objectTypes_.Resolver2< DotationType >::Get( dotationCapacity.GetName() );
            if( &category.GetLogisticSupplyClass() == &logType )
                requirements[ &category ] += agentComposition.GetCount() * dotationCapacity.GetNormalizedConsumption();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::GetQuantity
// Created: SLI 2014-02-19
// -----------------------------------------------------------------------------
unsigned int LogisticEditor::GetQuantity( const QStandardItemModel& dataModel, int row, double requirement )
{
    const double days = dataModel.item( row, 1 )->data( Qt::EditRole ).asDouble();
    return static_cast< unsigned int >( days * requirement + 0.5 );
}
