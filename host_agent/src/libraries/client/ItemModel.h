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
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QAbstractItemView>

namespace gui
{
    typedef boost::property_tree::ptree Tree;
}

namespace gui
{
enum ItemColumn
{
    ITEM_COL_TYPE,
    ITEM_COL_NAME,
    ITEM_COL_PACKAGE,
    ITEM_COL_VERSION,
    ITEM_COL_STATUS,
    ITEM_COL_DATE,
    ITEM_COL_CHECKSUM,
    ITEM_COL_SIZE,
    ITEM_COL_COUNT,
};

struct Item : public boost::noncopyable
{
     Item( const Tree& tree, int status );
     Item( size_t id, const QString& type, const QString& name, const QString& checksum );
    ~Item();

    QVariant        Data( int col, int role ) const;
    Qt::CheckState  GetCheckState() const;
    size_t          GetId() const;
    bool            Equal( size_t id ) const;
    bool            SetData( int col, const QVariant& value, int role );

private:
    const size_t    id_;
    const QString   type_;
    const QString   name_;
    const QString   package_;
    const QString   version_;
    const QDateTime date_;
    const QString   checksum_;

    QString         error_;
    int             status_; // [0,100[ downloading, >=100 complete
    size_t          size_;
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
    void Setup( QAbstractItemView* view );
    QAbstractItemModel* GetModel();

public slots:
    void Toggle();
    std::vector< size_t > Remove();

private:
    Qt::CheckState toggle_;
    QSortFilterProxyModel proxy_;
};
}

#endif // ITEMMODEL_H_