// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef FLATMODEL_H_
#define FLATMODEL_H_

#include <QAbstractItemModel>
#include <QVector>
#include <QStringList>
#include <boost/shared_ptr.hpp>

namespace gui
{
template< typename T >
struct FlatModel : public QAbstractItemModel
{
    // -----------------------------------------------------------------------------
    // Name: FlatModel::FlatModel
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    explicit FlatModel( QObject* parent = 0 )
        : QAbstractItemModel( parent )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::~FlatModel
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual ~FlatModel()
    {
        // NOTHING
    }

    typedef boost::shared_ptr< T > T_Ptr;
    typedef QVector< T_Ptr >       T_Items;
    typedef QStringList            T_Headers;

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Item
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    static T& Item( const QModelIndex& index )
    {
        return *static_cast< T* >( index.internalPointer() );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Append
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void Append( const T_Ptr& item )
    {
        Insert( items_.size(), item );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Insert
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void Insert( int row, const T_Ptr& item )
    {
        beginInsertRows( QModelIndex(), row, row );
        items_.insert( row, item );
        endInsertRows();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Replace
    // Created: BAX 2012-09-25
    // -----------------------------------------------------------------------------
    virtual void Replace( int row, const T_Ptr& item )
    {
        items_[row] = item;
        ModifyRow( row );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::ModifyRow
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void ModifyRow( int row )
    {
        emit dataChanged( index( row, 0 ),
                          index( row, headers_.size() - 1 ) );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::ModifyCell
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void ModifyCell( int row, int col )
    {
        emit dataChanged( index( row, col ),
                          index( row, col ) );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::ModifyColumn
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void ModifyColumn( int col )
    {
        emit dataChanged( index( 0, col ),
                          index( items_.size() - 1, col ) );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Remove
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void Remove( int row )
    {
        beginRemoveRows( QModelIndex(), row, row );
        items_.remove( row );
        endRemoveRows();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Clear
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual void Clear()
    {
        const int size = items_.size();
        if( !size )
            return;
        beginRemoveRows( QModelIndex(), 0, size - 1 );
        items_.clear();
        endRemoveRows();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::Find
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    template< typename U >
    QModelIndex Find( const U& target )
    {
        for( T_Items::const_iterator it = items_.begin(); it != items_.end(); ++it )
            if( (*it)->Equal( target ) )
                return index( std::distance< T_Items::const_iterator >( items_.begin(), it ), 0 );
        return QModelIndex();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::index
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual QModelIndex index( int row, int col, const QModelIndex& /*parent*/ = QModelIndex() ) const
    {
        if( row < 0 || row >= items_.size() )
            return QModelIndex();
        if( col < 0 || col >= headers_.size() )
            return QModelIndex();
        return createIndex( row, col, items_[row].get() );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::parent
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual QModelIndex parent( const QModelIndex& ) const
    {
        return QModelIndex();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::rowCount
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const
    {
        return parent.isValid() ? 0 : items_.size();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::columnCount
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual int columnCount( const QModelIndex& = QModelIndex() ) const
    {
        return headers_.size();
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::headerData
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
    {
        if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
            return QVariant();
        if( section < 0 || section >= headers_.size())
            return QVariant();
        return headers_[section];
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::data
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual QVariant data( const QModelIndex& index, int role ) const
    {
        return FlatModel< T >::Item( index ).Data( index.column(), role );
    }

    // -----------------------------------------------------------------------------
    // Name: FlatModel::setData
    // Created: BAX 2012-09-06
    // -----------------------------------------------------------------------------
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role )
    {
        const bool modified = FlatModel< T >::Item( index ).SetData( index.column(), value, role );
        if( modified )
            emit dataChanged( index, index );
        return modified;
    }

protected:
    T_Items items_;
    T_Headers headers_;
};
}

#endif // FLATMODEL_H_
