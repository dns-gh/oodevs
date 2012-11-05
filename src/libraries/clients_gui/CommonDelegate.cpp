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
#include "RichSpinBox.h"
#include "Roles.h"

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
// Name: CommonDelegate::UpdatePosition
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void CommonDelegate::CreatePosition( unsigned int id, int fromRow, int toRow, int fromCol, int toCol )
{
    // $$$$ ABR 2012-10-25: TODO Handle a priority management
    positions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddSpinBox( int fromRow, int toRow, int fromCol, int toCol, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/ )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    spinBoxs_[ id ] = SpinBoxDescription< int >( min, max, gap, 0 );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddDoubleSpinBox( int fromRow, int toRow, int fromCol, int toCol, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/ )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    doubleSpinBoxs_[ id ] = SpinBoxDescription< double >( min, max, gap, precision );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBox
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddComboBox( int fromRow, int toRow, int fromCol, int toCol, QStringList stringList )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    comboBoxs_[ id ] = QStringList( stringList );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddLineEdit
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddLineEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    lineEdits_.push_back( id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddCheckBox
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddCheckBox( int fromRow, int toRow, int fromCol, int toCol )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    checkBoxs_.push_back( id );
    return id;
}


// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetSpinBoxMinMax
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
template<>
void CommonDelegate::SetSpinBoxMinMax< int >( int row, int col, int min, int max )
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    if( !position )
        return;
    T_SpinBoxs::iterator it = spinBoxs_.find( position->id_ );
    if( it == spinBoxs_.end() )
        return;
    it->second.min_ = min;
    it->second.max_ = max;
}

template<>
void CommonDelegate::SetSpinBoxMinMax< double >( int row, int col, double min, double max )
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    if( !position )
        return;
    T_DoubleSpinBoxs::iterator it = doubleSpinBoxs_.find( position->id_ );
    if( it == doubleSpinBoxs_.end() )
        return;
    it->second.min_ = min;
    it->second.max_ = max;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetColumnsSumRestriction
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
template<>
void CommonDelegate::SetColumnsSumRestriction< int >( std::vector< int > columns, E_LinksType type, int sum )
{
    if( columns.size() > 1 )
        intRestrictions_.push_back( SumRestriction< int >( columns, type, sum ) );
}

template<>
void CommonDelegate::SetColumnsSumRestriction< double >( std::vector< int > columns, E_LinksType type, double sum )
{
    if( columns.size() > 1 )
        doubleRestrictions_.push_back( SumRestriction< double >( columns, type, sum ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetRowsSumRestriction
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
template<>
void CommonDelegate::SetRowsSumRestriction< int >( std::vector< int > rows, E_LinksType type, int sum )
{
    if( rows.size() > 1 )
        intRestrictions_.push_back( SumRestriction< int >( rows, type, sum, true ) );
}

template<>
void CommonDelegate::SetRowsSumRestriction< double >( std::vector< int > rows, E_LinksType type, double sum )
{
    if( rows.size() > 1 )
        doubleRestrictions_.push_back( SumRestriction< double >( rows, type, sum, true ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetColumnDependency
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void CommonDelegate::SetColumnDependency( int dependentColumn, int referenceColumn, E_LinksType dependencyType )
{
    if( dependentColumn != referenceColumn )
        dependencies_.push_back( Dependency( dependentColumn, referenceColumn, dependencyType ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetRowDependency
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
void CommonDelegate::SetRowDependency( int dependentRow, int referenceRow, E_LinksType dependencyType )
{
    if( dependentRow != referenceRow )
        dependencies_.push_back( Dependency( dependentRow, referenceRow, dependencyType, true ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::OnItemChanged
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
void CommonDelegate::OnItemChanged( QStandardItem* item )
{
    if( item && IsCheckBox( item->index() ) )
        emit CheckedStateChanged( *item );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::createEditor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
QWidget* CommonDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position || readOnly_ )
        return 0;

    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
    {
        std::pair< int, int > minMax = GetMinMax< int >( *element, newIndex, intRestrictions_ );
        QSpinBox* editor = new RichSpinBox( parent, minMax.first, minMax.second, element->gap_ );
        return editor;
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        std::pair< double, double > minMax = GetMinMax< double >( *element, newIndex, doubleRestrictions_ );
        QDoubleSpinBox* editor = new RichDoubleSpinBox( parent, minMax.first, minMax.second, element->gap_, element->precision_ );
        return editor;
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        QComboBox* editor = new QComboBox( parent );
        editor->addItems( *element );
        return editor;
    }
    else if( std::find( lineEdits_.begin(), lineEdits_.end(), position->id_ ) != lineEdits_.end() )
    {
        QLineEdit* editor = new QLineEdit( parent );
        return editor;
    }
    else if( std::find( checkBoxs_.begin(), checkBoxs_.end(), position->id_ ) != checkBoxs_.end() )
    {
        return 0;
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

    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
    {
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        int value = spinBox->locale().toInt( newIndex.model()->data( newIndex, Qt::EditRole ).toString() );
        spinBox->setValue( value );
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        double value = spinBox->locale().toDouble( newIndex.model()->data( newIndex, Qt::EditRole ).toString() );
        spinBox->setValue( value );
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        int value = newIndex.model()->data( newIndex, Roles::DataRole ).toInt();
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        comboBox->setCurrentIndex( value );
    }
    else if( std::find( lineEdits_.begin(), lineEdits_.end(), position->id_ ) != lineEdits_.end() )
    {
        QLineEdit* lineEdit = static_cast< QLineEdit* >( editor );
        lineEdit->setText( newIndex.model()->data( newIndex, Qt::EditRole ).toString() );
    }
    else if( std::find( checkBoxs_.begin(), checkBoxs_.end(), position->id_ ) != checkBoxs_.end() )
    {

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

    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
    {
        QSpinBox* spinBox = static_cast< QSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->locale().toString( spinBox->value() ), Qt::EditRole );
        model->setData( index, spinBox->value(), Roles::DataRole );
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        QDoubleSpinBox* spinBox = static_cast< QDoubleSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->locale().toString( spinBox->value(), 'f', element->precision_ ), Qt::EditRole );
        model->setData( index, spinBox->value(), Roles::DataRole );
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        QComboBox* comboBox = static_cast< QComboBox* >( editor );
        model->setData( index, comboBox->currentIndex(), Roles::DataRole );
        model->setData( index, comboBox->currentText(), Qt::EditRole );
    }
    else if( std::find( lineEdits_.begin(), lineEdits_.end(), position->id_ ) != lineEdits_.end() )
    {
        QLineEdit* editLine = static_cast< QLineEdit* >( editor );
        model->setData( index, editLine->text(), Roles::DataRole );
        model->setData( index, editLine->text(), Qt::EditRole );
    }
    else if( std::find( checkBoxs_.begin(), checkBoxs_.end(), position->id_ ) != checkBoxs_.end() )
    {

    }
}
