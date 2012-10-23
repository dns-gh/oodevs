// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyAvailabilityTableWidget.h"
#include "moc_LogisticSupplyAvailabilityTableWidget.cpp"
#include "clients_gui/RichSpinBox.h"
#include "clients_kernel/Tools.h"

using namespace gui;

namespace
{
    enum eColumnType
    {
        eName,
        eAvailable,
        eValue
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyAvailabilityItemDelegate::LogisticSupplyAvailabilityItemDelegate( QWidget* parent )
    : QItemDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyAvailabilityItemDelegate::~LogisticSupplyAvailabilityItemDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate::createEditor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
QWidget* LogisticSupplyAvailabilityItemDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index ) const
{
    LogisticSupplyAvailabilityTableWidget* parentTable = static_cast< LogisticSupplyAvailabilityTableWidget* >( this->parent() );
    if( !parentTable )
        return 0;
    parentTable->clearSelection();
    if( index.column() == eName )
    {
        QComboBox* comboBox = new QComboBox( parent );
        comboBox->addItem( QString() );
        const QAbstractItemModel* model = index.model();
        if( model )
        {
            QString dataName = index.model()->index( index.row(), eName ).data().value< QString >();
            if( !dataName.isEmpty() )
                comboBox->addItem( dataName );
            const QMap< QString, int >& maxQuantities = parentTable->GetMaxQuantities();
            for( QMap< QString, int >::const_iterator it = maxQuantities.begin(); it != maxQuantities.end(); ++it )
            {
                QString curName = it.key();
                if( !curName.isEmpty() && curName != dataName && !IsNameInTable( index.model(), curName ) )
                    comboBox->addItem( curName );
            }
        }
        connect( comboBox, SIGNAL( activated( const QString& ) ), parentTable, SLOT( OnNameChanged( const QString& ) ) );
        return comboBox;
    }
    else if( index.column() == eValue )
    {
        QString curName = index.model()->index( index.row(), eName ).data().value< QString >();
        if( curName.isEmpty() )
            return 0;
        int available = 0, qty = 0;
        const QAbstractItemModel* model = index.model();
        if( model )
        {
            int row = index.row();
            available = model->data( model->index( row, eAvailable ), Qt::UserRole ).value< int >();
            qty = model->data( model->index( row, eValue ), Qt::UserRole ).value< int >();
        }
        RichSpinBox* spinBox = new RichSpinBox( parent );
        spinBox->setRange( 0, available );
        spinBox->setValue( qty );
        spinBox->setSingleStep( 1 );
        return spinBox;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate::setEditorData
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityItemDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
    if( !editor )
        return;
    if( index.column() == eName )
    {
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        QString resource = index.model()->data( index, Qt::DisplayRole ).value< QString >();
        comboBox->setCurrentText( resource );
    }
    else if( index.column() == eValue )
    {
        RichSpinBox* spinBox = static_cast< RichSpinBox* >( editor );
        const QAbstractItemModel* model = index.model();
        if( model )
            spinBox->setValue( model->data( index, Qt::UserRole ).value< int >() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate::setModelData
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityItemDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
    if( !editor || !model )
        return;
    if( index.column() == eValue )
    {
        RichSpinBox* spinBox = static_cast< RichSpinBox* >( editor );
        int val = spinBox->value();
        model->setData( index, spinBox->text(), Qt::DisplayRole );
        model->setData( index, val, Qt::UserRole );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityItemDelegate::IsNameInTable
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
bool LogisticSupplyAvailabilityItemDelegate::IsNameInTable( const QAbstractItemModel* model, const QString& name ) const
{
    if( model )
        for( int i=0; i< model->rowCount(); ++i )
            if( model->index( i, eName ).data().value< QString >() == name )
                return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyAvailabilityTableWidget::LogisticSupplyAvailabilityTableWidget( QWidget* parent, const QStringList& header )
    : QTableWidget( parent )
    , header_( header )
{ 
    setColumnCount( 3 );
    setSelectionMode( QAbstractItemView::NoSelection );
    setItemDelegate( new LogisticSupplyAvailabilityItemDelegate( this ) );
    setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged );
    setColumnWidth( eName, 160 );
    setColumnWidth( eAvailable, 70 );
    setColumnWidth( eValue, 70 );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize( 25 );
    Clear();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyAvailabilityTableWidget::~LogisticSupplyAvailabilityTableWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget::Clear
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityTableWidget::Clear()
{
    clear();
    maxQuantities_.clear();
    setRowCount( 1 );
    setHorizontalHeaderLabels( header_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget::GetMaxQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
const QMap< QString, int >& LogisticSupplyAvailabilityTableWidget::GetMaxQuantities() const
{
    return maxQuantities_;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget::GetQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityTableWidget::GetQuantities( QMap< QString, int >& quantities ) const
{
    quantities.clear();
    QAbstractItemModel* model = this->model();
    if( model )
    {
        for( int i=0; i<model->rowCount(); ++i )
        {
            QString name = model->data( model->index( i, eName ), Qt::DisplayRole ).value< QString >();
            int quantity = model->data( model->index( i, eValue ), Qt::UserRole ).value< int >();
            if( !name.isEmpty() && quantity > 0 )
                quantities[ name ] = quantity;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget::SetQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityTableWidget::SetQuantities( QMap< QString, int >& quantities, const QMap< QString, int >& maxQuantities )
{
    Clear();
    maxQuantities_ = maxQuantities;
    if( quantities.isEmpty() )
        return;
    setRowCount( quantities.size() + 1 );
    int i = 0;
    for( QMap< QString, int >::iterator it = quantities.begin(); it != quantities.end(); ++it )
    {
        const QString& name = it.key();
        int quantity = it.value();
        int available = maxQuantities_[ name ];
        QAbstractItemModel* model = this->model();
        if( model && !name.isEmpty() && available > 0 && quantity > 0 )
        {
            QModelIndex nameIndex = model->index( i, eName );
            QModelIndex availableIndex = model->index( i, eAvailable );
            QModelIndex qtyIndex = model->index( i, eValue );
            model->setData( nameIndex, name, Qt::DisplayRole );
            model->setData( availableIndex, locale().toString( available ), Qt::DisplayRole );
            model->setData( availableIndex, available, Qt::UserRole );
            model->setData( qtyIndex, locale().toString( quantity ), Qt::DisplayRole );
            model->setData( qtyIndex, quantity, Qt::UserRole );
            item( i, eAvailable )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
            item( i, eValue )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
            ++i;
        }
    }
    setRowCount( i + 1 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyAvailabilityTableWidget::OnNameChanged
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyAvailabilityTableWidget::OnNameChanged( const QString& newName )
{
    QAbstractItemModel* model = this->model();
    if( !model )
        return;
    int rowsCount = rowCount();
    int row = currentIndex().row();
    QString previousName = model->data( model->index( row, eName ), Qt::DisplayRole ).value< QString >();
    bool isLastRow = ( row == ( rowsCount - 1 ) );
    if( newName.isEmpty() )
    {
        if( !isLastRow && !previousName.isEmpty() )
        {
            removeRow( row );
            clearSelection();
        }
    }
    else if( newName != previousName )
    {
        int available = maxQuantities_[ newName ];
        QModelIndex indexResource = model->index( row, eName );
        QModelIndex indexAvailable = model->index( row, eAvailable );
        QModelIndex indexQty = model->index( row, eValue );
        model->setData( indexResource, newName, Qt::DisplayRole );
        model->setData( indexAvailable, locale().toString( available ), Qt::DisplayRole );
        model->setData( indexAvailable, available, Qt::UserRole );
        model->setData( indexQty, locale().toString( 1 ), Qt::DisplayRole );
        model->setData( indexQty, 1, Qt::UserRole );
        item( row, eAvailable )->setText( locale().toString( available ) );
        item( row, eAvailable )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
        item( row, eValue )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
        setCurrentCell( row, 2, QItemSelectionModel::Select );
        clearSelection();
        if( isLastRow && previousName.isEmpty() )
            setRowCount( rowsCount + 1 );
    }
}
