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

template< typename T >
struct FlatModel : public QAbstractItemModel
{
    explicit FlatModel( QObject* parent = 0 )
        : QAbstractItemModel( parent )
    {
        // NOTHING
    }

    virtual ~FlatModel()
    {
        // NOTHING
    }

    typedef boost::shared_ptr< T > T_Ptr;
    typedef QVector< T_Ptr >       T_Items;
    typedef QStringList            T_Headers;

    static T& item( const QModelIndex& index )
    {
        return *static_cast< T* >( index.internalPointer() );
    }

    virtual void Append( const T_Ptr& item )
    {
        Insert( items_.size(), item );
    }

    virtual void Insert( int row, const T_Ptr& item )
    {
        beginInsertRows( QModelIndex(), row, row );
        items_.insert( row, item );
        endInsertRows();
    }

    virtual void Modify( int row )
    {
        emit dataChanged( index( row, 0 ),
                          index( row, headers_.size() ) );
    }

    virtual void Modify( int row, int col )
    {
        emit dataChanged( index( row, col ),
                          index( row, col ) );
    }

    virtual void Remove( int row )
    {
        beginRemoveRows( QModelIndex(), row, row );
        items_.remove( row );
        endRemoveRows();
    }

    virtual void Clear()
    {
        const int size = items_.size();
        if( !size )
            return;
        beginRemoveRows( QModelIndex(), 0, size - 1 );
        items_.clear();
        endRemoveRows();
    }

    template< typename U >
    QModelIndex Find( const U& target )
    {
        for( T_Items::const_iterator it = items_.begin(); it != items_.end(); ++it )
            if( (*it)->Equal( target ) )
                return index( std::distance( items_.begin(), it ), 0 );
        return QModelIndex();
    }

    virtual QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const
    {
        if( row < 0 || row >= items_.size() )
            return QModelIndex();
        if( col < 0 || col >= headers_.size() )
            return QModelIndex();
        return createIndex( row, col, items_[row].get() );
    }

    virtual QModelIndex parent( const QModelIndex& ) const
    {
        return QModelIndex();
    }

    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const
    {
        return parent.isValid() ? 0 : items_.size();
    }

    virtual int columnCount( const QModelIndex& = QModelIndex() ) const
    {
        return headers_.size();
    }

    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
    {
        if( orientation != Qt::Horizontal || role != Qt::DisplayRole )
            return QVariant();
        if( section < 0 || section >= headers_.size())
            return QVariant();
        return headers_[section];
    }

    virtual QVariant data( const QModelIndex& index, int role ) const
    {
        return FlatModel< T >::item( index ).data( index.column(), role );
    }

protected:
    T_Items items_;
    T_Headers headers_;
};

#endif // FLATMODEL_H_
