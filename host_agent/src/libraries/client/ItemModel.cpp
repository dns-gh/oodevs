// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ItemModel.h"
#include "Helpers.h"

#include "runtime/PropertyTree.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/static_assert.hpp>
#include <QDebug>
#include <QPixmap>

using namespace gui;
using namespace property_tree;

enum ItemColumn
{
    ITEM_COL_TYPE,
    ITEM_COL_NAME,
    ITEM_COL_PACKAGE,
    ITEM_COL_VERSION,
    ITEM_COL_DATE,
    ITEM_COL_CHECKSUM,
    ITEM_COL_SIZE,
    ITEM_COL_COUNT,
};

static const QString item_headers[] =
{
    "Type",
    "Name",
    "Package",
    "Version",
    "Date",
    "Checksum",
    "Size",
};

BOOST_STATIC_ASSERT( ITEM_COL_COUNT == COUNT_OF( item_headers ) );

namespace
{
QString QGet( const Tree& tree, const std::string& key )
{
    return Utf8( Get< std::string >( tree, key ) );
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
        return QString( "%1 GB" ).arg( n / mb );
    return QString( "%1 TB" ).arg( n / tb );
}
}

// -----------------------------------------------------------------------------
// Name: Item::Item
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Item::Item( const Tree& tree )
    : id_         ( Get< size_t >( tree, "id" ) )
    , type_       ( QGet( tree, "type" ) )
    , name_       ( QGet( tree, "name" ) )
    , package_    ( QGet( tree, "package" ) )
    , version_    ( QGet( tree, "version" ) )
    , date_       ( QDateTime::fromString( QGet( tree, "date" ), Qt::ISODate ) )
    , checksum_   ( QGet( tree, "checksum" ) )
    , size_       ( Get< uint64_t >( tree, "size" ) )
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

// -----------------------------------------------------------------------------
// Name: Item::Data
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
QVariant Item::Data( int col, int role )
{
    switch( role )
    {
        case Qt::DecorationRole:
            if( !col ) return QPixmap( ":/drive-harddisk.png" );
            break;

        case Qt::DisplayRole:
            switch( col )
            {
                case ITEM_COL_TYPE:     return Capitalize( type_ );
                case ITEM_COL_NAME:     return name_;
                case ITEM_COL_PACKAGE:  return package_;
                case ITEM_COL_VERSION:  return version_;
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

// -----------------------------------------------------------------------------
// Name: Item::SetData
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
bool Item::SetData( int col, const QVariant& value, int role )
{
    if( role != Qt::CheckStateRole )
        return false;
    check_state_ = static_cast< Qt::CheckState >( value.toInt() );
    return true;
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
// Name: Item::Equal
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
bool Item::Equal( size_t id ) const
{
    return id_ == id;
}

// -----------------------------------------------------------------------------
// Name: ItemModel::ItemModel
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
ItemModel::ItemModel()
    : toggle_( Qt::Checked )
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
        Append( boost::make_shared< gui::Item >( it->second ) );
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

    bool mixed = false;
    boost::optional< Qt::CheckState > def;
    BOOST_FOREACH( const T_Ptr& ptr, items_ )
        if( def == boost::none )
            def = ptr->GetCheckState();
        else
            mixed |= *def != ptr->GetCheckState();

    def = mixed ? toggle_ : Reverse( *def );
    BOOST_FOREACH( T_Ptr& ptr, items_ )
        ptr->SetData( ITEM_COL_TYPE, *def, Qt::CheckStateRole );
    ModifyColumn( ITEM_COL_TYPE );
    toggle_ = Reverse( *def );
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