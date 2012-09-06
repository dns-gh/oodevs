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
    bool SetData( int col, const QVariant& value, int role );

private:
    const size_t    id_;
    const QString   type_;
    const QString   name_;
    const QString   package_;
    const QString   version_;
    const QDateTime date_;
    const QString   checksum_;
    const size_t    size_;
    Qt::CheckState  check_state_;
};

class ItemModel : public FlatModel< gui::Item >
{
    Q_OBJECT

public:
     ItemModel();
    ~ItemModel();
    virtual Qt::ItemFlags flags( const QModelIndex& idx ) const;
    void Fill( const Tree& tree );

public slots:
    void Toggle();
    void Remove();

private:
    Qt::CheckState toggle_;
};
}

#endif // ITEMMODEL_H_