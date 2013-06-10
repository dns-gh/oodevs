// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CheckComboBox.h"
#include "moc_CheckComboBox.cpp"
#include "StandardModel.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace gui;

// =============================================================================
// CheckComboModel
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CheckComboModel constructors
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboModel::CheckComboModel( QObject* parent /*= 0*/ )
    : QStandardItemModel( 0, 1, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckComboModel destructor
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboModel::~CheckComboModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckComboModel::flags
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
Qt::ItemFlags CheckComboModel::flags( const QModelIndex& index ) const
{
    return QStandardItemModel::flags( index ) | Qt::ItemIsUserCheckable;
}

// -----------------------------------------------------------------------------
// Name: CheckComboModel::data
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
QVariant CheckComboModel::data( const QModelIndex& index, int role /* = Qt::DisplayRole */ ) const
{
    QVariant value = QStandardItemModel::data( index, role );
    if( index.isValid() && role == Qt::CheckStateRole && !value.isValid() )
        value = Qt::Unchecked;
    return value;
}

// -----------------------------------------------------------------------------
// Name: CheckComboModel::setData
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
bool CheckComboModel::setData( const QModelIndex& index, const QVariant& value, int role /* = Qt::EditRole */ )
{
    bool ok = QStandardItemModel::setData( index, value, role );
    if( ok && role == Qt::CheckStateRole )
    {
        emit dataChanged( index, index );
        emit checkStateChanged();
    }
    return ok;
}

// =============================================================================
// CheckComboLineEdit
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CheckComboLineEdit constructor
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboBox::CheckComboLineEdit::CheckComboLineEdit( const QString& objectName, QComboBox* parent /* = 0 */ )
    : RichLineEdit( objectName, parent )
    , parent_( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckComboLineEdit destructor
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboBox::CheckComboLineEdit::~CheckComboLineEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckComboLineEdit::mousePressEvent
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::CheckComboLineEdit::mousePressEvent( QMouseEvent* event )
{
    if( parent() )
        parent()->event( event );
}

// =============================================================================
// CheckComboBox
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CheckComboBox constructor
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboBox::CheckComboBox( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWidget< QComboBox >( objectName, parent )
    , lineEdit_ ( 0 )
    , separator_( ";" )
    , allText_  ( "" )
    , noneText_ ( "" )
{
    setModel( new CheckComboModel( this ) );
    connect( model(), SIGNAL( checkStateChanged() ), this, SLOT( UpdateCheckedItems() ) );
    connect( model(), SIGNAL( rowsInserted( const QModelIndex &, int, int ) ), this, SLOT( UpdateCheckedItems() ) );
    connect( model(), SIGNAL( rowsRemoved( const QModelIndex &, int, int ) ), this, SLOT( UpdateCheckedItems() ) );

    lineEdit_ = new CheckComboLineEdit( "lineEditComboBox", this );
    lineEdit_->setReadOnly( true );
    setLineEdit( lineEdit_ );
    setInsertPolicy( QComboBox::NoInsert );

    view()->installEventFilter( this );
    view()->window()->installEventFilter( this );
    view()->viewport()->installEventFilter( this );
    installEventFilter( this );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox destructor
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
CheckComboBox::~CheckComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::allText
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
QString CheckComboBox::AllText() const
{
    return allText_;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::setAllText
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::SetAllText( const QString& text )
{
    if( allText_ != text )
    {
        allText_ = text;
        UpdateCheckedItems();
    }
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::itemCheckState
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
Qt::CheckState CheckComboBox::ItemCheckState( int index ) const
{
    return static_cast< Qt::CheckState >( itemData( index, Qt::CheckStateRole ).toInt() );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::setItemCheckState
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::SetItemCheckState( int index, Qt::CheckState state )
{
    setItemData( index, state, Qt::CheckStateRole );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::separator
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
QString CheckComboBox::Separator() const
{
    return separator_;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::setSeparator
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::SetSeparator( const QString& separator )
{
    if( separator_ != separator )
    {
        separator_ = separator;
        UpdateCheckedItems();
    }
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::noneText
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
QString CheckComboBox::NoneText() const
{
    return noneText_;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::setNoneText
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::SetNoneText( const QString& noneText )
{
    noneText_ = noneText;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::checkedItems
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
QStringList CheckComboBox::CheckedItems() const
{
    QStringList items;
    if( model() )
    {
        QModelIndex index = model()->index( 0, modelColumn(), rootModelIndex() );
        QModelIndexList indexes = model()->match( index, Qt::CheckStateRole, Qt::Checked, -1, Qt::MatchExactly );
        for( QModelIndexList::const_iterator it = indexes.constBegin(); it != indexes.constEnd(); ++it )
            items += it->data().toString();
    }
    return items;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::setCheckedItems
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::SetCheckedItems( const QStringList& items )
{
    for( QStringList::const_iterator it = items.constBegin(); it != items.constEnd(); ++it )
    {
        const int index = findText( *it );
        SetItemCheckState(index, index != -1 ? Qt::Checked : Qt::Unchecked);
    }
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::toggleCheckState
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::ToggleCheckState( int index )
{
    QVariant value = itemData( index, Qt::CheckStateRole );
    if( value.isValid() )
    {
        Qt::CheckState state = static_cast< Qt::CheckState >( value.toInt() );
        setItemData( index, state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole );
    }
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::updateCheckedItems
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::UpdateCheckedItems()
{
    QStringList items = CheckedItems();
    if( items.isEmpty() || items.size() == count() )
        setEditText( allText_ );
    else
        setEditText( items.join( separator_ ) );

    emit CheckedItemsChanged( items );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::mousePressEvent
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::mousePressEvent( QMouseEvent* )
{
    showPopup();
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::ApplyFilter
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
bool CheckComboBox::ApplyFilter( QStandardItem& item, StandardModel& model ) const
{
    QStringList items = CheckedItems();
    if( items.empty() )
        return true;
    bool result = false;
    const kernel::Entity_ABC* entity = model.GetDataFromItem< kernel::Entity_ABC >( item );
    if( entity )
    {
        bool valid = true;
        bool empty = false;
        std::string extractedText = extractor_( *entity, valid, empty );
        if( !valid )
            return false;

        // $$$$ ABR 2012-06-19: TODO Make allText_ check action check and uncheck every item ( if( *it == allText_ ) )

        if( extractedText.find( ';' ) != std::string::npos )    // Multiple result
        {
            std::vector< std::string > extractedVector;
            boost::split( extractedVector, extractedText, boost::algorithm::is_any_of( ";" ) );

            for( QStringList::const_iterator it = items.constBegin(); !result && it != items.constEnd(); ++it )
                for( std::vector< std::string >::const_iterator extractedIt = extractedVector.begin(); !result && extractedIt != extractedVector.end(); ++extractedIt )
                    result = result || ( *it == noneText_ ) ? empty : *extractedIt == it->toUtf8().constData();
        }
        else                                                    // Single result
            for( QStringList::const_iterator it = items.constBegin(); !result && it != items.constEnd(); ++it )
                result = result || ( *it == noneText_ ) ? empty : extractedText == it->toUtf8().constData();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::SetExtractor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::SetExtractor( T_Extractor extractor )
{
    extractor_ = extractor;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::SetItemsCheckState
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::SetItemsCheckState( Qt::CheckState state )
{
    if( !model() )
        return;
    for( int index = 0; index < model()->rowCount(); ++index )
        SetItemCheckState( index, state );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromStringList
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::FillFromStringList( const std::vector< std::string >& vector )
{
    int row = 0;
    //AddItem( row, allText_ );
    AddItem( row, noneText_ );
    for( std::vector< std::string >::const_iterator it = vector.begin(); it != vector.end(); ++it )
        AddItem( row, it->c_str() );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromQStringList
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::FillFromQStringList( const QStringList& stringList )
{
    int row = 0;
    //AddItem( row, allText_ );
    AddItem( row, noneText_ );
    for( QStringList::const_iterator it = stringList.begin(); it != stringList.end(); ++it )
        AddItem( row, *it );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::AddItem
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::AddItem( int& row, const QString text )
{
    if( text.isEmpty() )
        return;
    QStandardItem* item = new QStandardItem( text );
    item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
    item->setData( Qt::Unchecked, Qt::CheckStateRole );
    static_cast< CheckComboModel* >( model() )->setItem( row++, 0, item );
}
