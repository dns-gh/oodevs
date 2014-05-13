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
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_gui/RichPushButton.h"
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
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "preparation/StaticModel.h"

Q_DECLARE_METATYPE( const kernel::LogisticSupplyClass* )

// -----------------------------------------------------------------------------
// Name: LogisticEditor constructor
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
LogisticEditor::LogisticEditor( QWidget* parent, const QString& objectName, kernel::Controllers& controllers, const StaticModel& staticModel )
    : QDialog( parent, "StocksEditionDialog", 0, Qt::WStyle_Customize | Qt::WStyle_Title )
    , staticModel_( staticModel )
    , controllers_( controllers )
    , selected_   ( controllers )
{
    setModal( true );
    gui::SubObjectName subObject( objectName );
    resize( 550, 350 );
    dataModel_ = new QStandardItemModel( this );
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( dataModel_ );
    proxyModel->setSortRole( Qt::UserRole + 1 );

    gui::CommonDelegate* delegate = new gui::CommonDelegate( this );
    delegate->AddDoubleSpinBoxOnColumn( eDays, 0, std::numeric_limits< double >::max() );

    tableView_ = new gui::RichWidget< QTableView >( "tableView", this );
    tableView_->setModel( proxyModel );
    tableView_->setItemDelegate( delegate );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->setAlternatingRowColors( true );
    tableView_->setSortingEnabled( true );
    tableView_->verticalHeader()->setVisible( false );

    gui::RichPushButton* okButton = new gui::RichPushButton( "ok", tr( "Ok" ), this );
    gui::RichPushButton* cancelButton = new gui::RichPushButton( "cancel", tr( "Cancel" ), this );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( tableView_ );
    layout->addLayout( buttonLayout );

    connect( dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnValueChanged( QStandardItem* ) ) );
    connect( okButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
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
// Name: LogisticEditor::Show
// Created: SLI 2014-02-21
// -----------------------------------------------------------------------------
void LogisticEditor::Show( const kernel::Entity_ABC& entity )
{
    if( entity.GetId() == 0 )
        return;
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );

    std::set< std::string > allowedSupplyClasses;
    logistic_helpers::VisitAgentsWithLogisticSupply( entity, [&]( const kernel::Agent_ABC& agent )
        {
            agent.GetType().GetAllowedSupplyClasses( allowedSupplyClasses );
        } );

    dataModel_->clear();
    dataModel_->setHorizontalHeaderLabels( QStringList() << tr( "Class" ) <<  tr( "Days" ) );
    tableView_->horizontalHeader()->setResizeMode( eCategory, QHeaderView::ResizeToContents );
    tableView_->horizontalHeader()->setResizeMode( eDays, QHeaderView::Stretch );
    auto itLogClass = staticModel_.objectTypes_.tools::StringResolver< kernel::LogisticSupplyClass >::CreateIterator();
    unsigned int row = 0;
    QStandardItem* item = 0;
    while( itLogClass.HasMoreElements() )
    {
        const kernel::LogisticSupplyClass& logClass = itLogClass.NextElement();
        if( allowedSupplyClasses.find( logClass.GetName() ) == allowedSupplyClasses.end() )
            continue;
        const QString text = logClass.GetName().c_str();
        item = new QStandardItem( text );
        item->setEditable( false );
        item->setCheckable( true );
        item->setData( QVariant::fromValue( &logClass ), Qt::UserRole );
        item->setData( text );
        item->setCheckState( Qt::Checked );
        dataModel_->setItem( row, eCategory, item );

        const double defaultValue = 1.;
        item = new QStandardItem( locale().toString( defaultValue, 'f', 2 ) );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
        item->setData( defaultValue );
        dataModel_->setItem( row, eDays, item );
        ++row;
    }
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::OnValueChanged
// Created: JSR 2011-10-04
// -----------------------------------------------------------------------------
void LogisticEditor::OnValueChanged( QStandardItem* item )
{
    if( !item )
        return;
    if( item->column() == eCategory )
    {
        if( QStandardItem* day = dataModel_->item( item->row(), eDays ) )
            day->setEnabled( item->checkState() == Qt::Checked );
    }
    else
        item->setData( item->data( Qt::EditRole ).asDouble() );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::Accept
// Created: MMC 2011-07-21
// -----------------------------------------------------------------------------
void LogisticEditor::Accept()
{
    SupplyHierarchy( selected_ );
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
// Name: LogisticEditor::EvaluateOneDaySupplyQty
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticEditor::SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity )
{
    if( !logistic_helpers::IsLogisticBase( *entity ) )
        return;
    const gui::LogisticHierarchiesBase* pLogHierarchy = entity->Retrieve< gui::LogisticHierarchiesBase >();
    if( !pLogHierarchy )
        return;
    SupplyHierarchy( *pLogHierarchy );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::FillSupplyRequirements
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticEditor::FillSupplyRequirements( const kernel::Entity_ABC& entity, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity );
    if( pAgent )
        ComputeRequirements( *pAgent, logType, requirements );
    const kernel::TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
    if( pTacticalHierarchies )
    {
        auto children = pTacticalHierarchies->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const kernel::Entity_ABC& childrenEntity = children.NextElement();
            FillSupplyRequirements( childrenEntity, logType, requirements );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::SupplyLogisticBaseStocks
// Created: MMC 2011-08-31
// -----------------------------------------------------------------------------
void LogisticEditor::SupplyLogisticBaseStocks( const kernel::Entity_ABC& blLogBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    auto logChildren = blLogBase.Get< gui::LogisticHierarchiesBase >().CreateSubordinateIterator();
    while( logChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = logChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( entity ) )
        {
            if( blLogBase.GetId() != entity.GetId() )
                SupplyLogisticBaseStocks( entity, logType, requirements );
        }
        else
            FillSupplyRequirements( entity, logType, requirements );
    }
    if( blLogBase.GetTypeName() == kernel::Automat_ABC::typeName_ && logistic_helpers::IsLogisticBase( blLogBase ) )
        FillSupplyRequirements( blLogBase, logType, requirements );
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::ComputeRequirements
// Created: MMC 2011-08-10
// -----------------------------------------------------------------------------
void LogisticEditor::ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements )
{
    kernel::AgentType& agentType = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( agent.GetType().GetId() );
    auto agentCompositionIterator = agentType.CreateIterator();
    while( agentCompositionIterator.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
        auto resourcesIterator = equipmentType.CreateResourcesIterator();
        while( resourcesIterator.HasMoreElements() )
        {
            const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
            const kernel::DotationType& category = staticModel_.objectTypes_.Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
            if( &category.GetLogisticSupplyClass() == &logType )
                requirements[ &category ] += static_cast< unsigned int >( agentComposition.GetCount() * dotationCapacity.GetNormalizedConsumption() + 0.5 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticEditor::GetDaysBySupplyClass
// Created: JSR 2014-03-10
// -----------------------------------------------------------------------------
std::map< const kernel::LogisticSupplyClass*, double > LogisticEditor::GetDaysBySupplyClass() const
{
    std::map< const kernel::LogisticSupplyClass*, double > result;
    for( int row = 0; row < dataModel_->rowCount(); ++row )
    {
        if( dataModel_->item( row )->checkState() == Qt::Checked )
        {
            auto supplyClass = dataModel_->item( row, eCategory )->data( Qt::UserRole ).value< const kernel::LogisticSupplyClass* >();
            result[ supplyClass ] = dataModel_->item( row, eDays )->data( Qt::EditRole ).asDouble();
        }
    }
    return result;
}
