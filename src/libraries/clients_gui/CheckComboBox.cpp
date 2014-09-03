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
        emit checkStateChanged( data( index, Roles::OtherRole ).toBool() );
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
CheckComboBox::CheckComboBox( const QString& objectName,
                              QWidget* parent,
                              bool useNone,
                              bool useCheckAll,
                              const T_Extractor& extractor,
                              const T_Filler& filler )
    : RichWidget< QComboBox >( objectName, parent )
    , dataModel_( new CheckComboModel( this ) )
    , proxyModel_( this )
    , extractor_( extractor )
    , lineEdit_ ( 0 )
    , separator_( ";" )
    , allText_  ( tr( "All" ) )
    , noneText_ ( useNone ? tr( "(None)" ) : "" )
    , checkAllText_ ( useCheckAll ? tr( "(Select all)" ) : "" )
{
    setModel( &proxyModel_ );
    proxyModel_.setSourceModel( dataModel_ );
    proxyModel_.setFilterRole( Roles::FilterRole );
    proxyModel_.setFilterRegExp( StandardModel::showValue_ );
    proxyModel_.setDynamicSortFilter( true );

    connect( dataModel_, SIGNAL( checkStateChanged( bool ) ), this, SLOT( UpdateCheckedItems( bool ) ) );
    connect( dataModel_, SIGNAL( rowsInserted( const QModelIndex &, int, int ) ), this, SLOT( UpdateCheckedItems() ) );
    connect( dataModel_, SIGNAL( rowsRemoved( const QModelIndex &, int, int ) ), this, SLOT( UpdateCheckedItems() ) );

    lineEdit_ = new CheckComboLineEdit( "lineEditComboBox", this );
    lineEdit_->setReadOnly( true );
    setLineEdit( lineEdit_ );
    setInsertPolicy( QComboBox::NoInsert );

    view()->installEventFilter( this );
    view()->window()->installEventFilter( this );
    view()->viewport()->installEventFilter( this );
    installEventFilter( this );
    filler( *this );
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
// Name: CheckComboBox::checkedItems
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
QStringList CheckComboBox::CheckedItems() const
{
    QStringList items;
    if( dataModel_ )
    {
        const QModelIndex index = dataModel_->index( 0, modelColumn(), rootModelIndex() );
        const QModelIndexList indexes = dataModel_->match( index, Qt::CheckStateRole, Qt::Checked, -1, Qt::MatchExactly );
        for( auto it = indexes.constBegin(); it != indexes.constEnd(); ++it )
        {
            auto text = it->data().toString();
            if( text != checkAllText_)
                items += text;
        }
    }
    return items;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::updateCheckedItems
// Created: ABR 2012-06-18
// -----------------------------------------------------------------------------
void CheckComboBox::UpdateCheckedItems( bool checkAll /* = false */ )
{
    QStringList items = CheckedItems();
    if( !checkAllText_.isEmpty() )
    {
        bool wasBlocked = dataModel_->blockSignals( true );
        if( checkAll )
            SetItemsCheckState( ItemCheckState( 0 ) == Qt::Checked ? Qt::Checked : Qt::Unchecked );
        else
            SetItemCheckState( 0, ( items.size() == proxyModel_.rowCount() - 1 ) ? Qt::Checked :
                                  ( items.size() == 0 ) ? Qt::Unchecked :
                                  Qt::PartiallyChecked );
        dataModel_->blockSignals( wasBlocked );
    }

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
// Name: CheckComboBox::Apply
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
bool CheckComboBox::Apply( QStandardItem& item ) const
{
    bool valid = true;
    bool empty = false;
    auto value = extractor_( item, valid, empty );
    if( !valid )
        return false;
    QStringList items = CheckedItems();
    if( items.empty() )
        return true;
    // $$$$ ABR 2012-06-19: TODO Make allText_ check action check and uncheck every item ( if( *it == allText_ ) )
    bool result = false;
    std::vector< std::string > values;
    boost::split( values, value, boost::algorithm::is_any_of( ";" ) );
    for( auto it = items.constBegin(); !result && it != items.constEnd(); ++it )
        for( auto extractedIt = values.begin(); !result && extractedIt != values.end(); ++extractedIt )
            result = result || ( *it == noneText_ ) ? empty : *extractedIt == it->toUtf8().constData();

    return result;
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::Clear
// Created: ABR 2014-04-25
// -----------------------------------------------------------------------------
void CheckComboBox::Clear()
{
    SetItemsCheckState( Qt::Unchecked );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::SetItemsCheckState
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::SetItemsCheckState( Qt::CheckState state )
{
    if( !dataModel_ )
        return;
    for( int index = 0; index < dataModel_->rowCount(); ++index )
        SetItemCheckState( index, state );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::FillFromStringList
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void CheckComboBox::FillFromStringList( const std::vector< std::string >& vector )
{
    int row = 0;
    AddItem( row, checkAllText_ );
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
    AddItem( row, checkAllText_ );
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
    auto flags = text == checkAllText_
        ? Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate
        : Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    item->setFlags( flags );
    item->setData( Qt::Unchecked, Qt::CheckStateRole );
    item->setData( QVariant( gui::StandardModel::showValue_ ), Roles::FilterRole );
    item->setData( text == checkAllText_, Roles::OtherRole );
    dataModel_->setItem( row++, 0, item );
}

// -----------------------------------------------------------------------------
// Name: CheckComboBox::CreateListView
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
QWidget* CheckComboBox::CreateListView( QWidget* parent )
{
    QListView* list = new RichWidget< QListView >( "checkcombobox-listview", parent );
    list->setModel( &proxyModel_ );
    return list;
}
