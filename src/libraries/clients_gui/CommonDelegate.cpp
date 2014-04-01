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
#include "RichWidget.h"
#include "RichLineEdit.h"
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
// Name: CommonDelegate::AddSimpleWidget
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddSimpleWidget( int fromRow, int toRow, int fromCol, int toCol, T_SimpleWidget& vector )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    vector.push_back( id );
    return id;
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
unsigned int CommonDelegate::AddLineEdit( int fromRow, int toRow, int fromCol, int toCol, QString regExp /*= ""*/ )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    lineEdits_[ id ] = regExp;
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDelayEdit
// Created: LGY 2014-03-26
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddDelayEdit( int fromRow, int toRow, int fromCol, int toCol, unsigned int min /*= 0u*/ )
{
    unsigned int id = GetNewId();
    CreatePosition( id, fromRow, toRow, fromCol, toCol );
    delayEdits_[ id ] = min;
    return id;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddCheckBox
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
unsigned int CommonDelegate::AddCheckBox( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, checkboxes_ );
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
// Name: CommonDelegate::SetSingleColumnSumRestriction
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
template<>
void CommonDelegate::SetSingleColumnSumRestriction< int >( int column, E_LinksType type, int sum )
{
    std::vector< int > columns;
    columns.push_back( column );
    singleIntRestrictions_.push_back( SumRestriction< int >( columns, type, sum ) );
}

template<>
void CommonDelegate::SetSingleColumnSumRestriction< double >( int column, E_LinksType type, double sum )
{
    std::vector< int > columns;
    columns.push_back( column );
    singleDoubleRestrictions_.push_back( SumRestriction< double >( columns, type, sum ) );
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
// Name: CommonDelegate::SetSingleRowSumRestriction
// Created: ABR 2012-11-07
// -----------------------------------------------------------------------------
template<>
void CommonDelegate::SetSingleRowSumRestriction< int >( int row, E_LinksType type, int sum )
{
    std::vector< int > rows;
    rows.push_back( row );
    singleIntRestrictions_.push_back( SumRestriction< int >( rows, type, sum, true ) );
}

template<>
void CommonDelegate::SetSingleRowSumRestriction< double >( int row, E_LinksType type, double sum )
{
    std::vector< int > rows;
    rows.push_back( row );
    singleDoubleRestrictions_.push_back( SumRestriction< double >( rows, type, sum, true ) );
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
    if( item && item->index().isValid() && IsCheckBox( item->index() ) )
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
        std::pair< int, int > minMax = GetMinMax< int >( *element, newIndex, intRestrictions_, singleIntRestrictions_ );
        RichSpinBox* editor = new RichSpinBox( "editorSpinBox", parent, minMax.first, minMax.second, element->gap_ );
        return editor;
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        std::pair< double, double > minMax = GetMinMax< double >( *element, newIndex, doubleRestrictions_, singleDoubleRestrictions_ );
        QDoubleSpinBox* editor = new RichDoubleSpinBox( "editorDoubleSpinBox", parent, minMax.first, minMax.second, element->gap_, element->precision_ );
        return editor;
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        RichWidget< QComboBox >* editor = new RichWidget< QComboBox >( "editorComboBox", parent );
        editor->addItems( *element );
        return editor;
    }
    else if( const QString* element = Find( lineEdits_, position->id_ ) )
    {
        RichLineEdit* editor = new RichLineEdit( "editorLineEdit", parent );
        return editor;
    }
    else if( std::find( checkboxes_.begin(), checkboxes_.end(), position->id_ ) != checkboxes_.end() )
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
        RichSpinBox* spinBox = static_cast< RichSpinBox* >( "spinbox", editor );
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
        RichWidget< QComboBox >* comboBox = static_cast< RichWidget< QComboBox >* >( editor );
        comboBox->setCurrentIndex( value );
    }
    else if( const QString* element = Find( lineEdits_, position->id_ ) )
    {
        RichLineEdit* lineEdit = static_cast< RichLineEdit* >( editor );
        if( !element->isEmpty() )
            lineEdit->setValidator( new QRegExpValidator( QRegExp( *element ), 0 ) );
        lineEdit->setText( newIndex.model()->data( newIndex, Qt::EditRole ).toString() );
    }
    else if( std::find( checkboxes_.begin(), checkboxes_.end(), position->id_ ) != checkboxes_.end() )
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
        RichSpinBox* spinBox = static_cast< RichSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->locale().toString( spinBox->value() ), Qt::EditRole );
        model->setData( index, spinBox->value(), Roles::DataRole );
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        RichDoubleSpinBox* spinBox = static_cast< RichDoubleSpinBox* >( editor );
        spinBox->interpretText();
        model->setData( index, spinBox->locale().toString( spinBox->value(), 'f', element->precision_ ), Qt::EditRole );
        model->setData( index, spinBox->value(), Roles::DataRole );
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        RichWidget< QComboBox >* comboBox = static_cast< RichWidget< QComboBox >* >( editor );
        model->setData( index, comboBox->currentIndex(), Roles::DataRole );
        model->setData( index, comboBox->currentText(), Qt::EditRole );
    }
    else if( const QString* element = Find( lineEdits_, position->id_ ) )
    {
        RichLineEdit* editLine = static_cast< RichLineEdit* >( editor );
        model->setData( index, editLine->text(), Roles::DataRole );
        model->setData( index, editLine->text(), Qt::EditRole );
    }
    else if( std::find( checkboxes_.begin(), checkboxes_.end(), position->id_ ) != checkboxes_.end() )
    {

    }
}
