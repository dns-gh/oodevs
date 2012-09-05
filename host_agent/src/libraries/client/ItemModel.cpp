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

static const struct { ItemColumn col; QString name; } item_headers[] =
{
    { ITEM_COL_TYPE,     "Type" },
    { ITEM_COL_NAME,     "Name" },
    { ITEM_COL_PACKAGE,  "Package" },
    { ITEM_COL_VERSION,  "Version" },
    { ITEM_COL_DATE,     "Date" },
    { ITEM_COL_CHECKSUM, "Checksum" },
};

BOOST_STATIC_ASSERT( ITEM_COL_COUNT == COUNT_OF( item_headers ) );

Item::Item()
{
    // NOTHING
}

Item::~Item()
{
    // NOTHING
}

QVariant Item::data( int col, int role )
{
    return QVariant();
}

ItemModel::ItemModel()
{
    for( size_t i = 0; i < COUNT_OF( item_headers ); ++i )
        headers_ << item_headers[i].name;
}

ItemModel::~ItemModel()
{
    // NOTHING
}
