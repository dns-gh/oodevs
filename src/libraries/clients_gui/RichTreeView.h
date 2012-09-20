// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __RichTreeView_h_
#define __RichTreeView_h_

#include <QtGui/qtreeview.h>
#include "clients_kernel/ReadOnlyModable.h"
#include "DragAndDropObserver_ABC.h"
#include "StandardModel.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class SearchTreeView_ABC;

// =============================================================================
/** @class  RichTreeView
    @brief  RichTreeView
*/
// Created: ABR 2012-08-10
// =============================================================================
class RichTreeView : public QTreeView
                   , public kernel::ReadOnlyModable
                   , public DragAndDropObserver_ABC
{
   Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichTreeView( kernel::Controllers& controllers, QWidget* parent = 0 );
    virtual ~RichTreeView();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void EnableDragAndDrop( bool enable );
//    virtual QStringList AdditionalMimeTypes() const { return QStringList(); }
    virtual void Drop( const QString& /*mimeType*/, void* /*data*/, QStandardItem& /*target*/ ) {}
    virtual QMimeData* MimeData( const QModelIndexList& /*indexes*/, bool& /*overriden*/ ) const { return 0; }
    //@}

    //! @name Locks
    //@{
    void SetCreationBlocked( bool creationBlocked );
    bool IsCreationBlocked() const;
    void SetContextMenuBlocked( bool contextMenuBlocked );
    bool IsContextMenuBlocked() const;
    //@}

    //! @name Filters/Sort
    //@{
    virtual void CreateFilters( SearchTreeView_ABC& searchTreeView );
    virtual bool LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const;
    void ApplyFilter( StandardModel::T_FilterFunction func );
    //@}

public slots:
    //! @name Search
    //@{
    void LockDragAndDrop( bool lock );
    void SearchAndSelect( const QString& searchedText );
    void SearchAndSelectNext();
    //@}

protected:
    //! @name Helpers
    //@{
    //@}

protected:
    //! @name Member data
    //@{
    QSortFilterProxyModel* proxyModel_;
    StandardModel dataModel_;
    QString searchedText_;
    bool creationBlocked_;
    bool contextMenuBlocked_;
    //@}
};

} //! namespace gui

#endif // __RichTreeView_h_
