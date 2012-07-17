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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "clients_gui/SpinTableItem.h"
#include "preparation/Dotation.h"
#include "preparation/DotationsItem.h"
#include "preparation/StaticModel.h"
#include "preparation/Stocks.h"
#include "tools/Iterator.h"

using namespace kernel;
using namespace gui;

QColor DotationsEditor::warningColor_( 200, 200, 200 );

// -----------------------------------------------------------------------------
// Name: DotationsEditor constructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::DotationsEditor( QDialog*& self, QWidget* parent, const ::StaticModel& staticModel )
    : ModalDialog( parent, "DotationsEditor" )
    , staticModel_( staticModel )
    , value_ ( 0 )
    , self_( self )
{
    self_ = this;
    setCaption( tr( "Resources editor" ) );
    setMinimumSize( 500, 500 );

    // Table
    table_ = new KeyPressEditableTable( 0, 2, this );
    table_->setColumnStretchable( 0, true );
    table_->setColumnWidth( 1, 100 );
    table_->horizontalHeader()->setLabel( 0, tr( "Type" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quantity" ) );

    // Info Panel
    infosLabel_ = new QLabel( this );
    infosTable_ = new Q3Table( 0, 4, this );
    infosTable_->setColumnStretchable( eWeightCurrent, true );
    infosTable_->setColumnStretchable( eWeightMax, true );
    infosTable_->setColumnStretchable( eVolumeCurrent, true );
    infosTable_->setColumnStretchable( eVolumeMax, true );
    infosTable_->horizontalHeader()->setLabel( eWeightCurrent, tr( "Current weight" ) );
    infosTable_->horizontalHeader()->setLabel( eWeightMax, tr( "Maximal weight" ) );
    infosTable_->horizontalHeader()->setLabel( eVolumeCurrent, tr( "Current volume" ) );
    infosTable_->horizontalHeader()->setLabel( eVolumeMax, tr( "Maximal volume" ) );

    // Buttons
    Q3HBox* buttonBox = new Q3HBox( this );
    buttonBox->setSpacing( 5 );
    QPushButton* clear  = new QPushButton( tr( "Clear" ), buttonBox );
    QPushButton* ok     = new QPushButton( tr( "Ok" ), buttonBox );
    QPushButton* cancel = new QPushButton( tr( "Cancel" ), buttonBox );

    // Layout
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this, 10, 5 );
    pMainLayout->addWidget( table_, 3 );
    pMainLayout->addWidget( infosLabel_ );
    pMainLayout->addWidget( infosTable_, 1 );
    pMainLayout->addWidget( buttonBox );

    types_.append( "" );
    tools::Iterator< const DotationType& > it = staticModel_.objectTypes_.Resolver2< DotationType >::CreateIterator();
    while( it.HasMoreElements() )
        types_.append( it.NextElement().GetName().c_str() );
    connect( infosLabel_, SIGNAL( linkActivated( const QString& ) ), SLOT( OnLinkActivated( const QString& ) ) );
    connect( clear , SIGNAL( clicked() ), SLOT( OnClear() ) );
    connect( ok    , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( table_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );
    show();
    OnLinkActivated( "hide" );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor destructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::~DotationsEditor()
{
    self_ = 0;
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
    (*value_)->Clear();
    for( int i = 0; i < table_->numRows() - 1; ++i )
        (*value_)->AddDotation( staticModel_.objectTypes_.Resolver2< DotationType >::Get( table_->text( i, 0 ).toUtf8().constData() ), locale().toUInt( table_->text( i, 1 ) ) );
    (*value_)->Update();
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
void DotationsEditor::SetCurrentItem( DotationsItem*& dotations, const Entity_ABC& current )
{
    value_ = &dotations;
    current_ = &current;
    table_->setNumRows( 0 );
    tools::Iterator< const Dotation& > it = dotations->CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        AddItem( &dotation );
    }
    AddItem();
    UpdateInfos();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::UpdateInfos
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
void DotationsEditor::UpdateInfos()
{
    if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( current_ ) )
    {
        // Compute max
        T_StockCapacities maxCapacities;
        tools::Iterator< const AgentComposition& > itComposition = agent->GetType().CreateIterator();
        while( itComposition.HasMoreElements() )
        {
            const AgentComposition& agentComposition = itComposition.NextElement();
            const ComponentType& equipment = agentComposition.GetType();
            const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver< EquipmentType >::Get( equipment.GetId() );
            if( const EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
            {
                double nEquipments = static_cast< double >( agentComposition.GetCount() );
                maxCapacities[ carrying->stockNature_ ].first += nEquipments * carrying->stockWeightCapacity_;
                maxCapacities[ carrying->stockNature_ ].second += nEquipments * carrying->stockVolumeCapacity_;
            }
        }
        // Compute current
        T_StockCapacities currentCapacities;
        for( int i = 0; i < table_->numRows() - 1; ++i )
        {
            if( table_->text( i, 1 ).isEmpty() )
                continue;
            bool ok = false;
            const double dotationQuantity = locale().toDouble( table_->text( i, 1 ), &ok );
            if( !ok )
                continue;
            const DotationType& dotationType = staticModel_.objectTypes_.Resolver2< DotationType >::Get( table_->text( i, 0 ).toStdString() );
            currentCapacities[ dotationType.GetNature() ].first += dotationQuantity * dotationType.GetUnitWeight();
            currentCapacities[ dotationType.GetNature() ].second += dotationQuantity * dotationType.GetUnitVolume();
        }
        // Fill missing value
        FillMissingWithZero( currentCapacities, maxCapacities );
        FillMissingWithZero( maxCapacities, currentCapacities );
        // Fill info table with value
        infosTable_->setNumRows( 0 );
        int row = 0;
        for( CIT_StockCapacities currentIt = currentCapacities.begin(); currentIt != currentCapacities.end(); ++currentIt, ++row )
        {
            CIT_StockCapacities maxIt = maxCapacities.find( currentIt->first );
            infosTable_->setNumRows( row + 1 );
            infosTable_->verticalHeader()->setLabel( row, currentIt->first.c_str() );
            AddInfoItem( row, eWeightCurrent, currentIt->second.first, eWeightMax, maxIt->second.first );
            AddInfoItem( row, eVolumeCurrent, currentIt->second.second, eVolumeMax, maxIt->second.second );
        }
    }
}

namespace
{
    class RightAlignTableItem : public Q3TableItem
    {
    public:
        RightAlignTableItem ( Q3Table * table, EditType et ) : Q3TableItem( table, et ), useColor_( false ) {}
        RightAlignTableItem ( Q3Table * table, EditType et, const QString & text ) : Q3TableItem( table, et, text ), useColor_( false ) {}

    public:
        void SetColor( QColor color ) { color_ = color; useColor_ = true; }

        virtual int alignment() const { return Qt::AlignRight; }
        virtual void paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected )
        {
            if( useColor_ )
            {
                QColorGroup newCg( cg );
                newCg.setColor( QColorGroup::Base, color_ );
                Q3TableItem::paint( p, newCg, cr, selected );
            }
            else
                Q3TableItem::paint( p, cg, cr, selected );
        }

    private:
        bool useColor_;
        QColor color_;
    };
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::AddInfoItem
// Created: ABR 2012-05-04
// -----------------------------------------------------------------------------
void DotationsEditor::AddInfoItem( int row, E_InfosColumns currentCol, double currentValue, E_InfosColumns maxCol, double maxValue )
{
    RightAlignTableItem* currentItem = new RightAlignTableItem( infosTable_, Q3TableItem::Never, locale().toString( currentValue, 'f', 2 ) );
    RightAlignTableItem* maxItem = new RightAlignTableItem( infosTable_, Q3TableItem::Never, locale().toString( maxValue, 'f', 2 ) );
    if( currentValue > maxValue )
        currentItem->SetColor( warningColor_ );
    infosTable_->setItem( row, currentCol, currentItem );
    infosTable_->setItem( row, maxCol, maxItem );
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
    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( col == 0 )
    {
        if( item.CurrentItem() == 0 && row != table_->numRows() - 1 )
        {
            table_->removeRow( row );
            table_->setCurrentCell( table_->numRows() - 1, 1 );
            return;
        }
        if( item.CurrentItem() && row == table_->numRows() - 1 )
        {
            const int current = item.CurrentItem();
            if( table_->numRows() < int( types_.size() ) - 1 )
                AddItem();
            item.SetCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
    }
    UpdateInfos();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::AddItem
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::AddItem( const Dotation* dotation /* =0*/ )
{
    const unsigned int row = table_->numRows();
    table_->setNumRows( row + 1 );
    ExclusiveComboTableItem* item = new ExclusiveComboTableItem( table_, types_ );
    table_->setItem( row, 0, item );
    table_->setItem( row, 1, new gui::SpinTableItem< int >( table_, 0, std::numeric_limits< int >::max(), 1 ) );
    if( dotation )
    {
        table_->setText( row, 1, locale().toString( dotation->quantity_ ) );
        item->SetCurrentText( dotation->type_.GetName().c_str() );
    }
    table_->setCurrentCell( row, 1 );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnClear
// Created: ABR 2012-05-04
// -----------------------------------------------------------------------------
void DotationsEditor::OnClear()
{
    table_->setNumRows( 0 );
    AddItem();
    UpdateInfos();
}
