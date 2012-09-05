// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef ITEMMODEL_H_
#define ITEMMODEL_H_

#include "FlatModel.h"

namespace gui
{
struct Item
{
     Item();
    ~Item();
    QVariant data( int col, int role );
};

struct ItemModel : FlatModel< Item >
{
     ItemModel();
    ~ItemModel();
};
}

#endif // ITEMMODEL_H_