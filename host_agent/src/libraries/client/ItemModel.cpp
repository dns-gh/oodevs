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
    return Q8( Get< std::string >( tree, key ).c_str() );
}

QString Capitalize( QString text )
{
    text[0] = text[0].toUpper();
    return text;
}

QString PrettySize( size_t n )
{
    static const size_t kb = 1000;
    static const size_t mb = kb * 1000;
    static const size_t gb = mb * 1000;
    static const size_t tb = gb * 1000;
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
    : id_      ( Get< size_t >( tree, "id" ) )
    , type_    ( QGet( tree, "type" ) )
    , name_    ( QGet( tree, "name" ) )
    , package_ ( QGet( tree, "package" ) )
    , version_ ( QGet( tree, "version" ) )
    , date_    ( QDateTime::fromString( QGet( tree, "date" ), Qt::ISODate ) )
    , checksum_( QGet( tree, "checksum" ) )
    , size_    ( Get< size_t >( tree, "size" ) )
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
    }
    return QVariant();
}

// -----------------------------------------------------------------------------
// Name: ItemModel::ItemModel
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
ItemModel::ItemModel()
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
void ItemModel::Fill( const Tree& tree )
{
    typedef std::pair< Tree::const_assoc_iterator, Tree::const_assoc_iterator > Range;
    Range range = tree.get_child( "items" ).equal_range( "" );
    for( Tree::const_assoc_iterator it = range.first; it != range.second; ++it )
        Append( boost::make_shared< gui::Item >( it->second ) );
}