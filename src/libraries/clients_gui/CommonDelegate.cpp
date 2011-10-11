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
    , currentId_( 0 )
    , readOnly_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate destructor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
CommonDelegate::~CommonDelegate()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddSpinBox( int fromRow, int toRow, int fromCol, int toCol,
                                         int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/,
                                         int minLinkedRow /*= -1*/, int maxLinkedRow /*= -1*/, int minLinkedCol /*= -1*/, int maxLinkedCol /*= -1*/ )
{
    assert( FindPosition( fromRow, toRow, fromCol, toCol ) == 0 );
    unsigned int id = GetNewId();
    positions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
    spinBoxs_.Register( id, *new SpinBoxDescription< int >( min, max, gap, 0, minLinkedRow, maxLinkedRow, minLinkedCol, maxLinkedCol ) );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddDoubleSpinBox( int fromRow, int toRow, int fromCol, int toCol,
                                               double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/,
                                               int minLinkedRow /*= -1*/, int maxLinkedRow /*= -1*/, int minLinkedCol /*= -1*/, int maxLinkedCol /*= -1*/ )
{
    assert( FindPosition( fromRow, toRow, fromCol, toCol ) == 0 );
    unsigned int id = GetNewId();
    positions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
    doubleSpinBoxs_.Register( id, *new SpinBoxDescription< double >( min, max, gap, precision, minLinkedRow, maxLinkedRow, minLinkedCol, maxLinkedCol ) );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddComboBox( int fromRow, int toRow, int fromCol, int toCol,
                                          QStringList stringList )
{
    assert( FindPosition( fromRow, toRow, fromCol, toCol ) == 0 );
    unsigned int id = GetNewId();
    positions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
    comboBoxs_.Register( id, *new QStringList( stringList ) );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::createEditor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
QWidget* CommonDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position || readOnly_ )
        return 0;

    if( SpinBoxDescription< int >* element = spinBoxs_.Find( position->id_ ) )
    {
        std::pair< int, int > minMax = GetMinMax< int >( *element, newIndex );
        QSpinBox* editor = new QSpinBox( parent );
        editor->setRange( minMax.first, minMax.second );
        editor->setSingleStep( element->gap_ );
        return editor;
    }
    else if( SpinBoxDescription< double >* element = doubleSpinBoxs_.Find( position->id_ ) )
    {
        std::pair< double, double > minMax = GetMinMax< double >( *element, newIndex );
        QDoubleSpinBox* editor = new QDoubleSpinBox( parent );
        editor->setRange( minMax.first, minMax.second );
        editor->setSingleStep( element->gap_ );
        editor->setDecimals( element->precision_ );
        return editor;
    }
    else if( QStringList* element = comboBoxs_.Find( position->id_ ) )
    {
        QComboBox* editor = new QComboBox( parent );
        editor->addItems( *element );
        return editor;
    }
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::setEditorData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void CommonDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position )
        return;

    if( SpinBoxDescription< int >* element = spinBoxs_.Find( position->id_ ) )
    {
        int value = newIndex.model()->data( newIndex, Qt::EditRole ).toInt();
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        spinBox->setValue( value );
    }
    else if( SpinBoxDescription< double >* element = doubleSpinBoxs_.Find( position->id_ ) )
    {
        double value = newIndex.model()->data( newIndex, Qt::EditRole ).toDouble();
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        spinBox->setValue( value );
    }
    else if( QStringList* element = comboBoxs_.Find( position->id_ ) )
    {
        int value = newIndex.model()->data( newIndex, Qt::UserRole ).toInt();
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
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position )
        return;

    if( SpinBoxDescription< int >* element = spinBoxs_.Find( position->id_ ) )
    {
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->value(), Qt::EditRole );
        model->setData( index, spinBox->value(), Qt::UserRole );
    }
    else if( SpinBoxDescription< double >* element = doubleSpinBoxs_.Find( position->id_ ) )
    {
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, QString::number( spinBox->value(), 'f', element->precision_ ), Qt::EditRole );
        model->setData( index, spinBox->value(), Qt::UserRole );
    }
    else if( QStringList* element = comboBoxs_.Find( position->id_ ) )
    {
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        model->setData( index, comboBox->currentIndex(), Qt::UserRole );
        model->setData( index, comboBox->currentText(), Qt::EditRole );
    }
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetSpinBoxMinMax
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
template< typename T >
void CommonDelegate::SetSpinBoxMinMax( int row, int col, T min, T max )
{
    assert( false );
}

template<>
void CommonDelegate::SetSpinBoxMinMax< int >( int row, int col, int min, int max )
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    if( !position )
        return;
    SpinBoxDescription< int >* element = spinBoxs_.Find( position->id_ );
    assert( element );
    element->min_ = min;
    element->max_ = max;
}

template<>
void CommonDelegate::SetSpinBoxMinMax< double >( int row, int col, double min, double max )
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    if( !position )
        return;
    SpinBoxDescription< double >* element = doubleSpinBoxs_.Find( position->id_ );
    assert( element );
    element->min_ = min;
    element->max_ = max;
}