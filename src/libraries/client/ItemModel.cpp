// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "client_pch.h"
#include "ItemModel.h"
#include "moc_ItemModel.cpp"
#include "Helpers.h"

#include "runtime/Helpers.h"
#include "runtime/PropertyTree.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/static_assert.hpp>

using namespace gui;
using namespace property_tree;

static const QString item_headers[] =
{
    "Type",
    "Name",
    "Package",
    "Version",
    "Status",
    "Date",
    "Checksum",
    "Size",
};

BOOST_STATIC_ASSERT( ITEM_COL_COUNT == COUNT_OF( item_headers ) );

namespace
{
QString QGet( const Tree& tree, const std::string& key )
{
    return QUtf8( Get< std::string >( tree, key ) );
}

QString Capitalize( QString text )
{
    text[0] = text[0].toUpper();
    return text;
}

QString PrettySize( uint64_t n )
{
    static const uint64_t kb = 1000;
    static const uint64_t mb = kb * 1000;
    static const uint64_t gb = mb * 1000;
    static const uint64_t tb = gb * 1000;
    if( n < kb )
        return QString( "%1 B" ).arg( n );
    if( n < mb )
        return QString( "%1 KB" ).arg( n / kb );
    if( n < gb )
        return QString( "%1 MB" ).arg( n / mb );
    if( n < tb )
        return QString( "%1 GB" ).arg( n / gb );
    return QString( "%1 TB" ).arg( n / tb );
}
}

// -----------------------------------------------------------------------------
// Name: Item::Item
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Item::Item( const Tree& tree, int status )
    : id_         ( Get< size_t >( tree, "id" ) )
    , type_       ( QGet( tree, "type" ) )
    , name_       ( QGet( tree, "name" ) )
    , package_    ( QGet( tree, "package" ) )
    , version_    ( QGet( tree, "version" ) )
    , status_     ( status )
    , date_       ( QDateTime::fromString( QGet( tree, "date" ), Qt::ISODate ) )
    , checksum_   ( QGet( tree, "checksum" ) )
    , required_   ( false )
    , size_       ( Get< int64_t >( tree, "size" ) )
    , check_state_( Qt::Unchecked )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Item::Item
// Created: BAX 2012-09-18
// -----------------------------------------------------------------------------
Item::Item( size_t id, const QString& type, const QString& name, const QString& checksum )
    : id_         ( id )
    , type_       ( type )
    , name_       ( name )
    , package_    ()
    , version_    ()
    , status_     ()
    , date_       ()
    , checksum_   ( checksum )
    , required_   ( true )
    , size_       ( 0 )
    , check_state_( Qt::Unchecked )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Item::~Item
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Item::~Item()
{
    // NOTHING
}

namespace
{
QVariant GetStatus( const QString& error, int status )
{
    if( !error.isEmpty() )
        return error;
    if( status < 100 )
        return status;
    return "Complete";
}
}

// -----------------------------------------------------------------------------
// Name: Item::Data
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
QVariant Item::Data( int col, int role ) const
{
    switch( role )
    {
        case Qt::DecorationRole:
            if( !col ) return QPixmap( ":/drive-harddisk.png" );
            break;

        case Qt::UserRole:
            switch( col )
            {
                case ITEM_COL_TYPE:     return type_;
                case ITEM_COL_NAME:     return name_;
                case ITEM_COL_PACKAGE:  return package_;
                case ITEM_COL_VERSION:  return version_;
                case ITEM_COL_STATUS:   return status_;
                case ITEM_COL_DATE:     return date_;
                case ITEM_COL_CHECKSUM: return checksum_;
                case ITEM_COL_SIZE:     return size_;
            }
            break;

        case Qt::DisplayRole:
            switch( col )
            {
                case ITEM_COL_TYPE:     return Capitalize( type_ );
                case ITEM_COL_NAME:     return name_;
                case ITEM_COL_PACKAGE:  return package_;
                case ITEM_COL_VERSION:  return version_;
                case ITEM_COL_STATUS:   return GetStatus( error_, status_ );
                case ITEM_COL_DATE:     return date_.toString( "yyyy-MM-dd hh:mm:ss" );
                case ITEM_COL_CHECKSUM: return "0x" + checksum_;
                case ITEM_COL_SIZE:     return PrettySize( size_ );
            }
            break;

        case Qt::CheckStateRole:
            if( !col ) return check_state_;
            break;
    }
    return QVariant();
}

namespace
{
// -----------------------------------------------------------------------------
// Name: SetProgress
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
bool SetProgress( int& current, int next )
{
    if( next <= current )
        return false;
    current = next;
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: Item::SetData
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
bool Item::SetData( int col, const QVariant& value, int role )
{
    switch( role )
    {
        case Qt::CheckStateRole:
            switch( col )
            {
                case ITEM_COL_TYPE:
                    check_state_ = static_cast< Qt::CheckState >( value.toInt() );
                    return true;
            }
            break;

        case Qt::UserRole:
            switch( col )
            {
                case ITEM_COL_STATUS:
                    return SetProgress( status_, value.toInt() );

                case ITEM_COL_SIZE:
                    size_ = static_cast< size_t >( value.toULongLong() );
                    return true;
            }
            break;

        case ErrorRole:
            error_ = value.toString();
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Item::GetCheckState
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Qt::CheckState Item::GetCheckState() const
{
    return check_state_;
}

// -----------------------------------------------------------------------------
// Name: Item::GetId
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
size_t Item::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Item::IsRequired
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
bool Item::IsRequired() const
{
    return required_;
}

// -----------------------------------------------------------------------------
// Name: Item::Equal
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
bool Item::Equal( size_t id ) const
{
    return id_ == id;
}

namespace
{
struct ItemProgressDelegate : public QStyledItemDelegate
{
    ItemProgressDelegate( QWidget* parent )
        : QStyledItemDelegate( parent )
    {
        // NOTHING
    }

    virtual ~ItemProgressDelegate()
    {
        // NOTHING
    }

    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
    {
        const int status = index.data( Qt::UserRole ).toInt();
        if( status < 0 || status >= 100 )
            return QStyledItemDelegate::paint( painter, option, index );
        painter->save();
        QApplication::style()->drawControl( QStyle::CE_ItemViewItem, &option, painter );
        QStyleOptionProgressBarV2 tmp;
        tmp.direction     = option.direction;
        tmp.fontMetrics   = option.fontMetrics;
        tmp.palette       = option.palette;
        tmp.rect          = option.rect;
        tmp.state         = option.state;
        tmp.minimum       = 0;
        tmp.maximum       = 100;
        tmp.progress      = status;
        tmp.text          = QString( "%1%" ).arg( tmp.progress );
        tmp.textAlignment = Qt::AlignRight;
        tmp.textVisible   = true;
        if( tmp.state & QStyle::State_Selected )
        {
            painter->fillRect(tmp.rect, tmp.palette.highlight());
            painter->setPen(tmp.palette.highlightedText().color());
        }
        QApplication::style()->drawControl( QStyle::CE_ProgressBar, &tmp, painter );
        painter->restore();
   }
};
}

// -----------------------------------------------------------------------------
// Name: ItemModel::ItemModel
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
ItemModel::ItemModel()
    : editable_( false )
{
    for( size_t i = 0; i < COUNT_OF( item_headers ); ++i )
        headers_ << item_headers[i];
}

// -----------------------------------------------------------------------------
// Name: ItemModel::~ItemModel
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
ItemModel::~ItemModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ItemModel::Setup
// Created: BAX 2012-09-17
// -----------------------------------------------------------------------------
void ItemModel::Setup( QAbstractItemView* view )
{
    proxy_.setSourceModel( this );
    proxy_.setFilterKeyColumn( -1 );
    proxy_.setFilterCaseSensitivity( Qt::CaseInsensitive );
    proxy_.setSortCaseSensitivity( Qt::CaseInsensitive );
    view->setItemDelegateForColumn( ITEM_COL_STATUS, new ItemProgressDelegate( view ) );
    view->setModel( &proxy_ );
}

// -----------------------------------------------------------------------------
// Name: ItemModel::GetModel
// Created: BAX 2012-09-17
// -----------------------------------------------------------------------------
QAbstractItemModel* ItemModel::GetModel()
{
    return &proxy_;
}

// -----------------------------------------------------------------------------
// Name: ItemModel::Fill
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Qt::ItemFlags ItemModel::flags( const QModelIndex& idx ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( idx );
    if( idx.column() == ITEM_COL_TYPE )
        flags |= Qt::ItemIsUserCheckable;
    return flags;
}

// -----------------------------------------------------------------------------
// Name: ItemModel::Fill
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void ItemModel::Fill( const Tree& tree )
{
    typedef std::pair< Tree::const_assoc_iterator, Tree::const_assoc_iterator > Range;
    Range range = tree.get_child( "items" ).equal_range( "" );
    for( Tree::const_assoc_iterator it = range.first; it != range.second; ++it )
        Append( boost::make_shared< gui::Item >( it->second, 100 ) );
}

namespace
{
Qt::CheckState Reverse( Qt::CheckState state )
{
    return state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
}
}

// -----------------------------------------------------------------------------
// Name: ItemModel::Toggle
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void ItemModel::Toggle()
{
    if( items_.empty() )
        return;

    boost::optional< Qt::CheckState > next;
    BOOST_FOREACH( const T_Ptr& ptr, items_ )
    {
        if( next == boost::none )
            next = Reverse( ptr->GetCheckState() );
        ptr->SetData( ITEM_COL_TYPE, *next, Qt::CheckStateRole );
    }
    ModifyColumn( ITEM_COL_TYPE );
}

// -----------------------------------------------------------------------------
// Name: ItemModel::Remove
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
std::vector< size_t > ItemModel::Remove()
{
    std::vector< size_t > rpy;
    BOOST_FOREACH( const T_Ptr& ptr, items_ )
        if( ptr->GetCheckState() == Qt::Checked )
            rpy.push_back( ptr->GetId() );
    // it is not possible to remove all items in one pass
    // as row indexes can be discontinuous...
    BOOST_FOREACH( size_t id, rpy )
        FlatModel< gui::Item >::Remove( Find( id ).row() );
    return rpy;
}

// -----------------------------------------------------------------------------
// Name: ItemModel::IsRequired
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
bool ItemModel::IsRequired( const QModelIndex& idx )
{
    return ItemModel::Item( idx ).IsRequired();
}

// -----------------------------------------------------------------------------
// Name: ItemModel::SetEditable
// Created: BAX 2012-10-04
// -----------------------------------------------------------------------------
void ItemModel::SetEditable( bool editable )
{
    editable_ = editable;
}

// -----------------------------------------------------------------------------
// Name: ItemModel::data
// Created: BAX 2012-10-04
// -----------------------------------------------------------------------------
QVariant ItemModel::data( const QModelIndex& index, int role ) const
{
    if( !editable_ && role == Qt::CheckStateRole )
        return QVariant();
    return Base::data( index, role );
}
