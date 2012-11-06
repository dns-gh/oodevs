// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TableDelegate.h"
#include "ADN_CheckBox.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_EditLine.h"
#include "ADN_StandardItem.h"
#include "ADN_TimeEdit.h"
#include "ADN_TimeField.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/VariantPointer.h"

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate constructor
// Created: ABR 2012-10-22
// -----------------------------------------------------------------------------
ADN_TableDelegate::ADN_TableDelegate( QObject* parent /*= 0*/ )
    : gui::CommonDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate destructor
// Created: ABR 2012-10-22
// -----------------------------------------------------------------------------
ADN_TableDelegate::~ADN_TableDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColor
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColor( int fromRow, int toRow, int fromCol, int toCol, double min, double max )
{
    unsigned int id = GetNewId();
    colorPositions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
    colors_[ id ] = std::pair< double, double >( min, max );
    return id;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorOnColumn
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorOnRow( int row, double min, double max )
{
    return AddColor( row, row, -1, -1, min, max );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorOnColumn
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorOnColumn( int col, double min, double max )
{
    return AddColor( -1, -1, col, col, min, max );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::GetColorType
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
const std::pair< double, double >* ADN_TableDelegate::GetColorType( int row, int col ) const
{
    for( CIT_Positions it = colorPositions_.begin(); it != colorPositions_.end(); ++it )
        if( ( it->fromRow_ == -1 || row >= it->fromRow_ ) &&
            ( it->toRow_ == -1   || row <= it->toRow_   ) &&
            ( it->fromCol_ == -1 || col >= it->fromCol_ ) &&
            ( it->toCol_ == -1   || col <= it->toCol_   ) )
            if( const std::pair< double, double >* color = Find( colors_, it->id_ ) )
                return color;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddDelayEditorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddDelayEditOnRow( int row )
{
    return AddDelayEdit( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddDelayEditorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddDelayEditOnColumn( int col )
{
    return AddDelayEdit( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddDelayEditor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddDelayEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, delayEdits_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEditOnRow( int row )
{
    return AddTimeEdit( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEditOnColumn( int col )
{
    return AddTimeEdit( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, timeEdits_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVectorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVectorOnRow( int row )
{
    return AddComboPtrInVector( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVectorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVectorOnColumn( int col )
{
    return AddComboPtrInVector( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVector
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVector( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, comboPtrInVectors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::createEditor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
QWidget* ADN_TableDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position || readOnly_ )
        return 0;

    ADN_Connector_ABC* data = const_cast< ADN_Connector_ABC* >( static_cast< const ADN_Connector_ABC* >( index.model()->data( index, gui::Roles::SafeRole ).value< kernel::VariantPointer >().ptr_ ) );
    if( !data )
        return 0;

    // $$$$ ABR 2012-10-25: TODO, Create ADN_SpinBox_Int and Double which will inherit gui::RichSpinBox, so we won't need to re-define setModelData below
    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
    {
        std::pair< int, int > minMax = GetMinMax< int >( *element, newIndex, intRestrictions_ );
        //ADN_SpinBox_Int* editor = new ADN_SpinBox_Int( parent, minMax.first, minMax.second, element->gap_ );
        ADN_EditLine_Int* editor = new ADN_EditLine_Int( parent );
        editor->GetValidator().setRange( minMax.first, minMax.second );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignLeft : Qt::AlignRight );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        std::pair< double, double > minMax = GetMinMax< double >( *element, newIndex, doubleRestrictions_ );

        //ADN_SpinBox_Double* editor = new ADN_SpinBox_Double( parent, minMax.first, minMax.second, element->gap_, element->precision_ );
        ADN_EditLine_Double* editor = new ADN_EditLine_Double( parent );
        editor->GetValidator().setRange( minMax.first, minMax.second );
        editor->GetValidator().setDecimals( element->precision_ );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignLeft : Qt::AlignRight );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        ADN_ComboBox_Enum* editor = new ADN_ComboBox_Enum( parent );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( comboPtrInVectors_.begin(), comboPtrInVectors_.end(), position->id_ ) != comboPtrInVectors_.end() )
    {
        ADN_ComboBox_Vector* editor = new ADN_ComboBox_Vector( parent );
        editor->GetConnector().Connect( static_cast< ADN_Connector_Vector_ABC* >( data ) );
        return editor;
    }
    else if( std::find( lineEdits_.begin(), lineEdits_.end(), position->id_ ) != lineEdits_.end() )
    {
        ADN_EditLine_String* editor = new ADN_EditLine_String( parent );
        editor->GetConnector().Connect( data );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignRight : Qt::AlignLeft );
        return editor;
    }
    else if( std::find( checkBoxs_.begin(), checkBoxs_.end(), position->id_ ) != checkBoxs_.end() )
    {
        return 0;
    }
    else if( std::find( delayEdits_.begin(), delayEdits_.end(), position->id_ ) != delayEdits_.end() )
    {
        ADN_TimeField* editor = new ADN_TimeField( parent );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( timeEdits_.begin(), timeEdits_.end(), position->id_ ) != timeEdits_.end() )
    {
        ADN_TimeEdit* editor = new ADN_TimeEdit( parent );
        editor->GetConnector().Connect( data );
        return editor;
    }
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::setEditorData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void ADN_TableDelegate::setEditorData( QWidget* /*editor*/, const QModelIndex& /*index*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::setModelData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void ADN_TableDelegate::setModelData( QWidget* editor, QAbstractItemModel* /*model*/, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    ADN_Connector_ABC* data = const_cast< ADN_Connector_ABC* >( static_cast< const ADN_Connector_ABC* >( index.model()->data( index, gui::Roles::SafeRole ).value< kernel::VariantPointer >().ptr_ ) );
    if( !position || !data )
        return;
    ADN_Connector_ABC* guiConnetor = 0;
    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
        guiConnetor = &static_cast< ADN_EditLine_Int* >( editor )->GetConnector();
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
        guiConnetor = &static_cast< ADN_EditLine_Double* >( editor )->GetConnector();
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
        guiConnetor = &static_cast< ADN_ComboBox_Enum* >( editor )->GetConnector();
    else if( std::find( comboPtrInVectors_.begin(), comboPtrInVectors_.end(), position->id_ ) != comboPtrInVectors_.end() )
        guiConnetor = &static_cast< ADN_ComboBox_Vector* >( editor )->GetConnector();
    else if( std::find( lineEdits_.begin(), lineEdits_.end(), position->id_ ) != lineEdits_.end() )
        guiConnetor = &static_cast< ADN_EditLine_String* >( editor )->GetConnector();
    else if( std::find( delayEdits_.begin(), delayEdits_.end(), position->id_ ) != delayEdits_.end() )
        guiConnetor = &static_cast< ADN_TimeField* >( editor )->GetConnector();
    else if( std::find( timeEdits_.begin(), timeEdits_.end(), position->id_ ) != timeEdits_.end() )
        guiConnetor = &static_cast< ADN_TimeEdit* >( editor )->GetConnector();
    if( guiConnetor )
        guiConnetor->Disconnect( data );
}
