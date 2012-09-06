// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ItemModel.h"

#include <boost/static_assert.hpp>

using namespace gui;

enum ItemColumn
{
    ITEM_COL_TYPE,
    ITEM_COL_NAME,
    ITEM_COL_PACKAGE,
    ITEM_COL_VERSION,
    ITEM_COL_DATE,
    ITEM_COL_CHECKSUM,
    ITEM_COL_COUNT,
};

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

static const QString item_headers[] =
{
    "Type",
    "Name",
    "Package",
    "Version",
    "Date",
    "Checksum",
};

BOOST_STATIC_ASSERT( ITEM_COL_COUNT == COUNT_OF( item_headers ) );

// -----------------------------------------------------------------------------
// Name: Item::Item
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Item::Item()
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
// Name: Item::data
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
QVariant Item::data( int col, int role )
{
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
