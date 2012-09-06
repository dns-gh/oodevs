// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __StandardModel_h_
#define __StandardModel_h_

#include <QtGui/qstandarditemmodel.h>
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/VariantPointer.h"
#include <boost/function.hpp>

namespace gui
{

// =============================================================================
/** @class  StandardModel
    @brief  StandardModel
*/
// Created: ABR 2012-08-13
// =============================================================================
class StandardModel : public QStandardItemModel
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_InternalRole{ DataRole = Qt::UserRole, FilterRole = Qt::UserRole + 1, SortRole = Qt::UserRole + 2 };
    //@}

    //! @name Static values
    //@{
    static const QString showValue_;
    static const QString hideValue_;
    //@}

signals:
    //! @name Signals
    //@{
    void DataChanged( const QModelIndex& index, const QVariant& value );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             StandardModel( kernel::Controllers& controllers, QSortFilterProxyModel& proxy, QObject* parent = 0 );
    virtual ~StandardModel();
    //@}

    //! @name Operations
    //@{
    virtual bool setData ( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

    void Purge();
    template< typename T >
    void PurgeObsoleteSafeItem();

    QStandardItem* AddRootItem( int row, int col, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildItem( QStandardItem* parent, int row, int col, Qt::ItemFlags flags = 0 );

    QStandardItem* AddRootTextItem( int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildTextItem( QStandardItem* parent, int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags = 0 );

    template< typename T >
    QStandardItem* AddRootSafeItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );
    template< typename T >
    QStandardItem* AddChildSafeItem( QStandardItem* parent, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );

    QStandardItem* AddRootIconItem( int row, int col, const QPixmap& pixmap, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildIconItem( QStandardItem* parent, int row, int col, const QPixmap& pixmap, Qt::ItemFlags flags = 0 );
    //@}

    //! @name Accessors
    //@{
    template< typename T >
    T* GetDataFromItem( const QStandardItem& item ) const;
    template< typename T >
    T* GetDataFromIndex( const QModelIndex& index ) const;
    //@}

    //! @name Find
    //@{
    template< typename T >
    T* FindSafeData( const T& value );

    QStandardItem* FindItem( const QString& text );
    template< typename T >
    QStandardItem* FindSafeItem( const T& value );
    //@}

    //! @name Filters
    //@{
    void ApplyFilter( boost::function< bool ( QStandardItem* ) > func );
    //@}

private:
    //! @name helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QSortFilterProxyModel& proxy_;
    //@}
};

#include "StandardModel.inl"

} //! namespace gui

#endif // __StandardModel_h_
