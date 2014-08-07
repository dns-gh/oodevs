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

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/VariantPointer.h"
#include "Roles.h"
#include "Filter_ABC.h"

namespace gui
{
class DragAndDropObserver_ABC;
class ItemDecorationGetter_ABC;
class StandardModelVisitor_ABC;

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
    typedef std::function< bool ( QStandardItem& ) > T_Filter;
    //@}

    //! @name Static values
    //@{
    static const QString showValue_;
    static const QString hideValue_;
    static const QString& GetShowValue( bool visible ) { return visible ? showValue_ : hideValue_; }
    //@}

signals:
    //! @name Signals
    //@{
    void DataChanged( const QModelIndex& index, const QVariant& value );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             StandardModel( kernel::Controllers* controllers, QSortFilterProxyModel& proxy, QObject* parent = 0 );
    virtual ~StandardModel();
    //@}

    //! @name Operations
    //@{
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );

    void SetDecorationGetter( ItemDecorationGetter_ABC* decorationGetter );
    void SetDragAndDropObserver( DragAndDropObserver_ABC* dragAndDropObserver );

    void Accept( StandardModelVisitor_ABC& visitor, QStandardItem* root = 0 );

    void Purge();
    template< typename T >
    void PurgeObsoleteSafeItem();
    void DeleteTree( QStandardItem* item );
    void DeleteItemRow( QStandardItem* item );
    void PurgeChildren( QStandardItem& item );
    void MoveItem( QStandardItem& item, QStandardItem& newParent );

    QStandardItem* AddRootItem( int row, int col, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildItem( QStandardItem* root, int row, int col, Qt::ItemFlags flags = 0 );

    QStandardItem* AddRootTextItem( int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildTextItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags = 0 );

    template< typename T >
    QStandardItem* AddRootSafeItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );
    template< typename T >
    QStandardItem* AddChildSafeItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );

    template< typename T >
    QStandardItem* AddRootDataItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );
    template< typename T >
    QStandardItem* AddChildDataItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags = 0 );

    QStandardItem* AddRootIconItem( int row, int col, const QPixmap& pixmap, Qt::ItemFlags flags = 0 );
    QStandardItem* AddChildIconItem( QStandardItem* root, int row, int col, const QPixmap& pixmap, Qt::ItemFlags flags = 0 );

    QModelIndex GetMainModelIndex( const QModelIndex& index ) const;
    //@}

    //! @name Accessors
    //@{
    template< typename T >
    T* GetDataFromItem( const QStandardItem& item ) const;
    template< typename T >
    T* GetDataFromIndex( const QModelIndex& index ) const;
    QStandardItem* GetItemFromIndex( const QModelIndex& index ) const;
    //@}

    //! @name Find
    //@{
    template< typename T >
    T* FindData( const T& value, QStandardItem* root = 0 ) const;

    QStandardItem* FindTextItem( const QString& text, QStandardItem* root = 0 ) const;
    template< typename T >
    QStandardItem* FindDataItem( const T& value, QStandardItem* root = 0 ) const;
    //@}

    //! @name Filters
    //@{
    void ApplyFilter( const T_Filter& filter, int col = 0 ) const;
    void ApplyFilters( const std::map< int, std::vector< std::shared_ptr< Filter_ABC > > >& filters ) const;
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
    void InternalApplyFilters( int row, int col, const std::vector< std::shared_ptr< Filter_ABC > >& filters, bool& result ) const;
    bool HasAnyChildVisible( QStandardItem& root, const T_Filter& filter, int col ) const;
    bool HasAnyColumnVisible( int col, const std::vector< std::shared_ptr< Filter_ABC > >& filters ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers* controllers_;
    QSortFilterProxyModel& proxy_;
    DragAndDropObserver_ABC* dragAndDropObserver_;
    bool dndLocked_;
    ItemDecorationGetter_ABC* decorationGetter_;
    //@}
};

#include "StandardModel.inl"

} //! namespace gui

#endif // __StandardModel_h_
