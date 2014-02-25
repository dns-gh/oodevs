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
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Tools.h"
#include "gaming/Dotation.h"

namespace
{
    enum eColumnType
    {
        eName,
        eAvailable,
        eValue,
        eMass,
        eVolume
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
        int available = 0, quantity = 0;
        const QAbstractItemModel* model = index.model();
        if( model )
        {
            int row = index.row();
            available = model->data( model->index( row, eAvailable ), Qt::UserRole ).value< int >();
            quantity = model->data( model->index( row, eValue ), Qt::UserRole ).value< int >();
        }
        gui::RichSpinBox* spinBox = new gui::RichSpinBox( "spinBox", parent );
        spinBox->setRange( 0, available );
        spinBox->setValue( quantity );
        spinBox->setSingleStep( 1 );
        connect( spinBox, SIGNAL( valueChanged( int ) ), parentTable, SLOT( OnQuantityChanged() ) );
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
        gui::RichSpinBox* spinBox = static_cast< gui::RichSpinBox* >( editor );
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
        gui::RichSpinBox* spinBox = static_cast< gui::RichSpinBox* >( editor );
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
LogisticSupplyAvailabilityTableWidget::LogisticSupplyAvailabilityTableWidget( QWidget* parent, const QStringList& header, const T_AvailableDotations& dotations )
    : QTableWidget( parent )
    , header_( header )
    , dotations_( dotations )
{
    setColumnCount( header.size() );
    setSelectionMode( QAbstractItemView::NoSelection );
    setItemDelegate( new LogisticSupplyAvailabilityItemDelegate( this ) );
    setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked | QAbstractItemView::CurrentChanged );
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
void LogisticSupplyAvailabilityTableWidget::SetQuantities( const QMap< QString, int >& quantities, const QMap< QString, int >& maxQuantities )
{
    Clear();
    maxQuantities_ = maxQuantities;
    if( quantities.isEmpty() )
        return;
    setRowCount( quantities.size() + 1 );
    int i = 0;
    for( auto it = quantities.begin(); it != quantities.end(); ++it )
    {
        const QString& name = it.key();
        int quantity = it.value();
        int available = maxQuantities_[ name ];
        QAbstractItemModel* model = this->model();
        if( model && !name.isEmpty() && available > 0 && quantity > 0 )
            AddRow( i++, name, available, quantity );
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
    const int rowsCount = rowCount();
    const int row = currentIndex().row();
    const QString previousName = model->data( model->index( row, eName ), Qt::DisplayRole ).value< QString >();
    const bool isLastRow = ( row == ( rowsCount - 1 ) );
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
        AddRow( row, newName, available, 1 );
        setCurrentCell( row, 2, QItemSelectionModel::Select );
        clearSelection();
        if( isLastRow && previousName.isEmpty() )
            setRowCount( rowsCount + 1 );
    }
    emit OnChanged( row );
}

void LogisticSupplyAvailabilityTableWidget::OnQuantityChanged()
{
    UpdateMassVolume( currentIndex().row() );
    emit OnChanged( currentIndex().row() );
}

void LogisticSupplyAvailabilityTableWidget::AddRow( int row, const QString& newName, int available, int quantity )
{
    SetContent( row, eName, newName );
    SetContent( row, eAvailable, available );
    SetContent( row, eValue, quantity );
    UpdateMassVolume( row );
}

void LogisticSupplyAvailabilityTableWidget::UpdateMassVolume( int row )
{
    const QString name = model()->data( model()->index( row, eName ) ).asString();
    const int available = model()->data( model()->index( row, eValue ), Qt::UserRole ).value< int >();
    auto it = dotations_.find( name );
    if( it != dotations_.end() )
    {
        SetContent( row, eMass, available * it->second.type_->GetUnitWeight() );
        SetContent( row, eVolume, available * it->second.type_->GetUnitVolume() );
    }
}

void LogisticSupplyAvailabilityTableWidget::SetContent( int row, int col, int value )
{
    const QModelIndex index = model()->index( row, col );
    model()->setData( index, value, Qt::DisplayRole );
    model()->setData( index, value, Qt::UserRole );
    item( row, col )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
}

void LogisticSupplyAvailabilityTableWidget::SetContent( int row, int col, double value )
{
    const QModelIndex index = model()->index( row, col );
    model()->setData( index, locale().toString( value, 'f', 2 ), Qt::DisplayRole );
    model()->setData( index, value, Qt::UserRole );
    item( row, col )->setTextAlignment( Qt::AlignVCenter | Qt::AlignRight );
}

void LogisticSupplyAvailabilityTableWidget::SetContent( int row, int col, const QString& value, const QString& warning )
{
    const QModelIndex index = model()->index( row, col );
    model()->setData( index, value, Qt::DisplayRole );
    const auto i = item( row, col );
    i->setTextAlignment( Qt::AlignVCenter | ( col ? Qt::AlignRight : Qt::AlignLeft ) );
    i->setTextColor( warning.isEmpty() ? Qt::black : Qt::red );
    i->setToolTip( warning );
}
