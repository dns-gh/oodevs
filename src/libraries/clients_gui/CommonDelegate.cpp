// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CommonDelegate.h"
#include "moc_CommonDelegate.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CommonDelegate constructor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
CommonDelegate::CommonDelegate( QObject* parent /*= 0*/ )
    : QItemDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate destructor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
CommonDelegate::~CommonDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBox
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::AddSpinBox( int column, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/, int minLinkedColumn /*= -1*/, int maxLinkedColumn /*= -1*/ )
{
    assert( spinBoxs_.find( column ) == spinBoxs_.end() && doubleSpinBoxs_.find( column ) == doubleSpinBoxs_.end() && comboBoxs_.find( column ) == comboBoxs_.end() );
    spinBoxs_[ column ] = SpinBoxDescription< int >( min, max, gap, 0, minLinkedColumn, maxLinkedColumn );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBox
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::AddDoubleSpinBox( int column, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/, int minLinkedColumn /*= -1*/, int maxLinkedColumn /*= -1*/ )
{
    assert( spinBoxs_.find( column ) == spinBoxs_.end() && doubleSpinBoxs_.find( column ) == doubleSpinBoxs_.end() && comboBoxs_.find( column ) == comboBoxs_.end() );
    doubleSpinBoxs_[ column ] = SpinBoxDescription< double >( min, max, gap, precision, minLinkedColumn, maxLinkedColumn );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBox
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::AddComboBox( int column, QStringList stringList )
{
    assert( spinBoxs_.find( column ) == spinBoxs_.end() && doubleSpinBoxs_.find( column ) == doubleSpinBoxs_.end() && comboBoxs_.find( column ) == comboBoxs_.end() );
    comboBoxs_[ column ] = stringList;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::createEditor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
QWidget* CommonDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
{
    if( spinBoxs_.find( index.column() ) != spinBoxs_.end() )
    {
        CIT_SpinBoxs it = spinBoxs_.find( index.column() );
        int minimum = it->second.min_;
        if( it->second.minLinkedColumn_ != -1 )
            minimum = static_cast< const QStandardItemModel* >( index.model() )->item( index.row(), it->second.minLinkedColumn_ )->data( Qt::EditRole ).toInt();
        int maximum = it->second.max_;
        if( it->second.maxLinkedColumn_ != -1 )
            maximum = static_cast< const QStandardItemModel* >( index.model() )->item( index.row(), it->second.maxLinkedColumn_ )->data( Qt::EditRole ).toInt();

        QSpinBox* editor = new QSpinBox( parent );
        editor->setRange( minimum, maximum );
        editor->setSingleStep( it->second.gap_ );
        return editor;
    }
    else if( doubleSpinBoxs_.find( index.column() ) != doubleSpinBoxs_.end() )
    {
        CIT_DoubleSpinBoxs it = doubleSpinBoxs_.find( index.column() );
        double minimum = it->second.min_;
        if( it->second.minLinkedColumn_ != -1 )
            minimum = static_cast< const QStandardItemModel* >( index.model() )->item( index.row(), it->second.minLinkedColumn_ )->data( Qt::EditRole ).toDouble();
        double maximum = it->second.max_;
        if( it->second.maxLinkedColumn_ != -1 )
            maximum = static_cast< const QStandardItemModel* >( index.model() )->item( index.row(), it->second.maxLinkedColumn_ )->data( Qt::EditRole ).toDouble();

        QDoubleSpinBox* editor = new QDoubleSpinBox( parent );
        editor->setRange( minimum, maximum );
        editor->setSingleStep( it->second.gap_ );
        editor->setDecimals( it->second.precision_ );
        return editor;
    }
    else if( comboBoxs_.find( index.column() ) != comboBoxs_.end() )
    {
        CIT_ComboBoxs it = comboBoxs_.find( index.column() );
        QComboBox* editor = new QComboBox( parent );
        editor->addItems( it->second );
        return editor;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::setEditorData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
    if( spinBoxs_.find( index.column() ) != spinBoxs_.end() )
    {
        int value = index.model()->data( index, Qt::EditRole ).toInt();
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        spinBox->setValue( value );
    }
    else if( doubleSpinBoxs_.find( index.column() ) != doubleSpinBoxs_.end() )
    {
        double value = index.model()->data( index, Qt::EditRole ).toDouble();
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        spinBox->setValue( value );
    }
    else if( comboBoxs_.find( index.column() ) != comboBoxs_.end() )
    {
        int value = index.model()->data( index, Qt::EditRole ).toInt();
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        comboBox->setCurrentIndex( value );
    }
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::setModelData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
    if( spinBoxs_.find( index.column() ) != spinBoxs_.end() )
    {
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->value(), Qt::EditRole );
    }
    else if( doubleSpinBoxs_.find( index.column() ) != doubleSpinBoxs_.end() )
    {
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->value(), Qt::EditRole );
    }
    else if( comboBoxs_.find( index.column() ) != comboBoxs_.end() )
    {
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        model->setData( index, comboBox->currentText(), Qt::DisplayRole );
        model->setData( index, comboBox->currentIndex(), Qt::EditRole );
    }
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::updateEditorGeometry
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index */ ) const
{
    editor->setGeometry(option.rect);
}
