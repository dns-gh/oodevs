// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DotationsEditor.h"
#include "moc_DotationsEditor.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/SubObjectName.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/Dotation.h"
#include "preparation/DotationsItem.h"
#include "preparation/Stocks.h"

#define WARNING_COLOR QColor( 200, 200, 200 )

// -----------------------------------------------------------------------------
// Name: DotationsEditor constructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::DotationsEditor( QWidget* parent, const kernel::ObjectTypes& objectTypes )
    : gui::PropertyDialog( parent )
    , equipments_( objectTypes )
    , dotations_( objectTypes )
    , value_( 0 )
{
    gui::SubObjectName subObject( "DotationsEditor" );
    setCaption( tr( "Resources editor" ) );
    setMinimumSize( 500, 500 );

    // Table
    table_ = new gui::RichWidget< QTableWidget >( "table" );
    gui::CommonDelegate* delegate = new gui::CommonDelegate( this );
    table_->setItemDelegate( delegate );
    table_->setColumnCount( 2 );
    table_->horizontalHeader()->setStretchLastSection( false );
    table_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    table_->horizontalHeader()->setResizeMode( 1, QHeaderView::Fixed );
    table_->setColumnWidth( 1, 100 );
    QStringList tableHeaders;
    tableHeaders << tr( "Type" ) << tr( "Quantity" );
    table_->setHorizontalHeaderLabels( tableHeaders );

    // Info Panel
    infosLabel_ = new QLabel( this );
    infosTable_ = new gui::RichWidget< QTableWidget >( "infoTable" );
    infosTable_->setSelectionMode( QAbstractItemView::NoSelection );
    infosTable_->setColumnCount( 4 );
    infosTable_->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList headers;
    headers << tr( "Current weight" ) << tr( "Maximal weight" ) << tr( "Current volume" ) << tr( "Maximal volume" );
    infosTable_->setHorizontalHeaderLabels( headers );

    // Buttons
    Q3HBox* buttonBox = new Q3HBox( this );
    buttonBox->setSpacing( 5 );
    gui::RichPushButton* clear  = new gui::RichPushButton( "clear", tr( "Clear" ), buttonBox );
    gui::RichPushButton* ok     = new gui::RichPushButton( "ok", tr( "Ok" ), buttonBox );
    gui::RichPushButton* cancel = new gui::RichPushButton( "cancel", tr( "Cancel" ), buttonBox );

    // Layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this, 10, 5 );
    pMainLayout->addWidget( table_, 3 );
    pMainLayout->addWidget( infosLabel_ );
    pMainLayout->addWidget( infosTable_, 1 );
    pMainLayout->addWidget( buttonBox );

    QStringList types;
    types.append( "" );
    tools::Iterator< const kernel::DotationType& > it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
        types.append( it.NextElement().GetName().c_str() );
    delegate->AddComboBoxOnColumn( 0, types );
    delegate->AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );

    connect( infosLabel_, SIGNAL( linkActivated( const QString& ) ), SLOT( OnLinkActivated( const QString& ) ) );
    connect( clear , SIGNAL( clicked() ), SLOT( OnClear() ) );
    connect( ok    , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( table_, SIGNAL( cellChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );
    OnLinkActivated( "hide" );

}

// -----------------------------------------------------------------------------
// Name: DotationsEditor destructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::~DotationsEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnLinkActivated
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
void DotationsEditor::OnLinkActivated( const QString& link )
{
    if( link == "display" )
    {
        infosLabel_->setText( "<a href='hide'>" + tr( "Hide information" ) + "</a>" );
        infosTable_->setVisible( true );
    }
    else if( link == "hide" )
    {
        infosLabel_->setText( "<a href='display'>" + tr( "Display information" ) + "</a>" );
        infosTable_->setVisible( false );
    }
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnAccept
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnAccept()
{
    ( *value_ )->Clear();
    for( int i = 0; i < table_->rowCount() - 1; ++i )
        ( *value_ )->AddDotation( dotations_.Get( table_->item( i, 0 )->text().toStdString() ), locale().toUInt( table_->item( i, 1 )->text() ) );
    ( *value_ )->Update();
    accept();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnReject
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::SetCurrentItem
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::SetCurrentItem( DotationsItem*& dotations, const kernel::Entity_ABC& current )
{
    value_ = &dotations;
    current_ = &current;
    table_->clearContents();
    table_->setRowCount( dotations->CountDotations() + 1 );
    tools::Iterator< const Dotation& > it = dotations->CreateIterator();
    int row = 0;
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        AddItem( row++, &dotation );
    }
    AddItem( row );
    UpdateInfos();
    OnLinkActivated( "hide" );
    infosLabel_->setVisible( dotations->IsStock() );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::UpdateInfos
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
void DotationsEditor::UpdateInfos()
{
    if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( current_ ) )
    {
        // Compute max
        T_StockCapacities maxCapacities;
        tools::Iterator< const kernel::AgentComposition& > itComposition = agent->GetType().CreateIterator();
        while( itComposition.HasMoreElements() )
        {
            const kernel::AgentComposition& agentComposition = itComposition.NextElement();
            const kernel::ComponentType& equipment = agentComposition.GetType();
            const kernel::EquipmentType& equipmentType = equipments_.Get( equipment.GetId() );
            if( const kernel::EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
            {
                double nEquipments = static_cast< double >( agentComposition.GetCount() );
                maxCapacities[ carrying->stockNature_ ].first += nEquipments * carrying->stockWeightCapacity_;
                maxCapacities[ carrying->stockNature_ ].second += nEquipments * carrying->stockVolumeCapacity_;
            }
        }
        // Compute current
        T_StockCapacities currentCapacities;
        for( int i = 0; i < table_->rowCount() - 1; ++i )
        {
            if( table_->item( i, 1 )->text().isEmpty() )
                continue;
            bool ok = false;
            const double dotationQuantity = locale().toDouble( table_->item( i, 1 )->text(), &ok );
            if( !ok )
                continue;
            const kernel::DotationType& dotationType = dotations_.Get( table_->item( i, 0 )->text().toStdString() );
            currentCapacities[ dotationType.GetNature() ].first += dotationQuantity * dotationType.GetUnitWeight();
            currentCapacities[ dotationType.GetNature() ].second += dotationQuantity * dotationType.GetUnitVolume();
        }
        // Fill missing value
        FillMissingWithZero( currentCapacities, maxCapacities );
        FillMissingWithZero( maxCapacities, currentCapacities );
        // Fill info table with value
        infosTable_->clearContents();
        QStringList verticalHeaders;
        int row = 0;
        infosTable_->setRowCount( static_cast< int >( currentCapacities.size() ) );
        for( CIT_StockCapacities currentIt = currentCapacities.begin(); currentIt != currentCapacities.end(); ++currentIt, ++row )
        {
            CIT_StockCapacities maxIt = maxCapacities.find( currentIt->first );
            verticalHeaders << currentIt->first.c_str();
            AddInfoItem( row, eWeightCurrent, currentIt->second.first, eWeightMax, maxIt->second.first );
            AddInfoItem( row, eVolumeCurrent, currentIt->second.second, eVolumeMax, maxIt->second.second );
        }
        infosTable_->setVerticalHeaderLabels( verticalHeaders );
    }
}

namespace
{
    QTableWidgetItem* MakeItem( QWidget* w, double value )
    {
        QTableWidgetItem* item = new QTableWidgetItem( w->locale().toString( value, 'f', 2 ) );
        item->setTextAlignment( Qt::AlignRight );
        item->setFlags( item->flags() ^ Qt::ItemIsEditable );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::AddInfoItem
// Created: ABR 2012-05-04
// -----------------------------------------------------------------------------
void DotationsEditor::AddInfoItem( int row, E_InfosColumns currentCol, double currentValue, E_InfosColumns maxCol, double maxValue )
{
    infosTable_->setItem( row, currentCol, MakeItem( this, currentValue ) );
    infosTable_->setItem( row, maxCol, MakeItem( this, maxValue ) );
    if( currentValue > maxValue )
        infosTable_->item( row, currentCol )->setBackgroundColor( WARNING_COLOR );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::FillMissingWithZero
// Created: ABR 2012-05-04
// -----------------------------------------------------------------------------
void DotationsEditor::FillMissingWithZero( DotationsEditor::T_StockCapacities& src, DotationsEditor::T_StockCapacities& dst )
{
    for( CIT_StockCapacities srcIt = src.begin(); srcIt != src.end(); ++srcIt )
        if( dst.find( srcIt->first ) == dst.end() )
            dst[ srcIt->first ] = std::pair< double, double >( 0., 0. );
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver< Dotation >* DotationsEditor::GetValue
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsItem* DotationsEditor::GetValue()
{
    return *value_;
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnValueChanged
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnValueChanged( int row, int col )
{
    if( col == 0 )
    {
        QTableWidgetItem* item = table_->item( row, 0 );
        if( item->text().isEmpty() && row != table_->rowCount() - 1 )
        {
            table_->removeRow( row );
            table_->setCurrentCell( table_->rowCount() - 1, 1, QItemSelectionModel::ClearAndSelect );
            return;
        }
        if( !item->text().isEmpty() && row == table_->rowCount() - 1 && table_->rowCount() < static_cast< int >( dotations_.Count() - 1 ) )
        {
            const int rowCount = table_->rowCount();
            table_->setRowCount( rowCount + 1 );
            AddItem( rowCount );
        }
        table_->setCurrentCell( row, 1 );
    }
    UpdateInfos();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::AddItem
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::AddItem( int row, const Dotation* dotation /* =0*/ )
{
    table_->blockSignals( true );
    table_->setItem( row, 0, new QTableWidgetItem() );
    table_->setItem( row, 1, new QTableWidgetItem() );
    if( dotation )
    {
        table_->item( row, 0 )->setText( dotation->type_.GetName().c_str() );
        table_->item( row, 1 )->setText( locale().toString( dotation->quantity_ ) );
    }
    else
        table_->item( row, 1 )->setText( locale().toString( 0 ) );
    table_->setCurrentCell( row, 1 );
    table_->blockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnClear
// Created: ABR 2012-05-04
// -----------------------------------------------------------------------------
void DotationsEditor::OnClear()
{
    table_->clearContents();
    table_->setRowCount( 1 );
    AddItem( 0 );
    UpdateInfos();
}
