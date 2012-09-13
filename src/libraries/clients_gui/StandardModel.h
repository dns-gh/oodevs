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
class DragAndDropObserver_ABC;

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
    static const QString mimeTypeStr_;
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

    void SetDragAndDropObserver( DragAndDropObserver_ABC* dragAndDropObserver );

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

    QModelIndex GetMainModelIndex( const QModelIndex& index ) const;
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

    QStandardItem* FindItem( const QString& text, QStandardItem* root = 0 );
    template< typename T >
    QStandardItem* FindSafeItem( const T& value );
    //@}

    //! @name Filters
    //@{
    void ApplyFilter( boost::function< bool ( QStandardItem* ) > func );
    //@}

    //! @name Drag and Drop
    //@{
    void LockDragAndDrop( bool lock );
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData( const QModelIndexList& indexes ) const;
    virtual bool dropMimeData( const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent );
    virtual Qt::DropActions supportedDropActions() const;
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
    DragAndDropObserver_ABC* dragAndDropObserver_;
    bool dndLocked_;
    //@}
};

#include "StandardModel.inl"

} //! namespace gui

#endif // __StandardModel_h_
