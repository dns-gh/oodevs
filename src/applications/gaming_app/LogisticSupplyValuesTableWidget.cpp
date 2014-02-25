// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticSupplyValuesTableWidget.h"
#include "moc_LogisticSupplyValuesTableWidget.cpp"
#include "clients_gui/RichSpinBox.h"
#include "clients_kernel/Tools.h"

using namespace gui;

namespace
{
    enum eColumnType
    {
        eName,
        eValue
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValueItemDelegate constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyValueItemDelegate::LogisticSupplyValueItemDelegate( QWidget* parent )
    : QItemDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValueItemDelegate destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyValueItemDelegate::~LogisticSupplyValueItemDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValueItemDelegate::createEditor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
QWidget* LogisticSupplyValueItemDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index ) const
{
    LogisticSupplyValuesTableWidget* parentTable = static_cast< LogisticSupplyValuesTableWidget* >( this->parent() );
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
            const QStringList& possible = parentTable->GetPossible();
            for( QStringList::const_iterator it = possible.begin(); it != possible.end(); ++it )
            {
                QString curName = *it;
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
        int quantity = 0;
        const QAbstractItemModel* model = index.model();
        if( model )
        {
            int row = index.row();
            quantity = model->data( model->index( row, eValue ), Qt::UserRole ).value< int >();
        }
        RichSpinBox* spinBox = new RichSpinBox( "spinBox", parent );
        spinBox->setRange( 0, std::numeric_limits< int >::max() );
        spinBox->setValue( quantity );
        spinBox->setSingleStep( 1 );
        return spinBox;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyExclusiveListWidget::setEditorData
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValueItemDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
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
// Name: LogisticSupplyExclusiveListWidget::setModelData
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValueItemDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
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
// Name: LogisticSupplyExclusiveListWidget::IsNameInTable
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
bool LogisticSupplyValueItemDelegate::IsNameInTable( const QAbstractItemModel* model, const QString& name ) const
{
    if( model )
        for( int i=0; i< model->rowCount(); ++i )
            if( model->index( i, eName ).data().value< QString >() == name )
                return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget constructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyValuesTableWidget::LogisticSupplyValuesTableWidget( QWidget* parent, const QStringList& header )
    : QTableWidget( parent )
    , header_( header )
{
    setColumnCount( 2 );
    setSelectionMode( QAbstractItemView::NoSelection );
    setItemDelegate( new LogisticSupplyValueItemDelegate( this ) );
    setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged );
    setColumnWidth( eName, 255 );
    setColumnWidth( eValue, 100 );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize( 25 );
    Clear();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget destructor
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
LogisticSupplyValuesTableWidget::~LogisticSupplyValuesTableWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget::Clear
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValuesTableWidget::Clear()
{
    clear();
    possible_.clear();
    setRowCount( 1 );
    setHorizontalHeaderLabels( header_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget::GetMaxQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
const QStringList& LogisticSupplyValuesTableWidget::GetPossible() const
{
    return possible_;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget::GetQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValuesTableWidget::GetQuantities( QMap< QString, int >& quantities ) const
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
// Name: LogisticSupplyValuesTableWidget::SetQuantities
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValuesTableWidget::SetQuantities( QMap< QString, int >& quantities, QStringList& possible )
{
    Clear();
    possible_ = possible;
    if( quantities.isEmpty() )
        return;
    setRowCount( quantities.size() + 1 );
    int i = 0;
    for( QMap< QString, int >::iterator it = quantities.begin(); it != quantities.end(); ++it )
    {
        const QString& name = it.key();
        int quantity = it.value();
        QAbstractItemModel* model = this->model();
        if( model && !name.isEmpty() && quantity > 0 && ( possible.find( name ) != possible.end() )  )
        {
            model->setData( model->index( i, eName ), name, Qt::DisplayRole );
            const QModelIndex index = model->index( i, eValue );
            model->setData( index, locale().toString( quantity ), Qt::DisplayRole );
            model->setData( index, quantity, Qt::UserRole );
            item( i, eValue )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
            ++i;
        }
    }
    setRowCount( i + 1 );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyValuesTableWidget::OnNameChanged
// Created: MMC 2012-10-11
// -----------------------------------------------------------------------------
void LogisticSupplyValuesTableWidget::OnNameChanged( const QString& newName )
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
        const QModelIndex index = model->index( row, eValue );
        model->setData( model->index( row, eName ), newName, Qt::DisplayRole );
        model->setData( index, locale().toString( 1 ), Qt::DisplayRole );
        model->setData( index, 1, Qt::UserRole );
        item( row, eValue )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
        setCurrentCell( row, 2, QItemSelectionModel::Select );
        clearSelection();
        if( isLastRow && previousName.isEmpty() )
            setRowCount( rowsCount + 1 );
    }
}
