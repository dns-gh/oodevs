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
#include <boost/property_tree/ptree_fwd.hpp>
#include <QDateTime>

namespace gui
{
    typedef boost::property_tree::ptree Tree;
}

namespace gui
{
struct Item
{
     Item( const Tree& tree );
    ~Item();

    QVariant Data( int col, int role );

private:
    const size_t  id_;
    const QString type_;
    const QString name_;
    const QString package_;
    const QString version_;
    const QDate   date_;
    const QString checksum_;
    const QString briefing_;
};

struct ItemModel : FlatModel< Item >
{
     ItemModel();
    ~ItemModel();
    void Fill( const Tree& tree );
};
}

#endif // ITEMMODEL_H_