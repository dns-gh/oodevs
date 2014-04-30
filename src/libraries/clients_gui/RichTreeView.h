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

#include "DragAndDropObserver_ABC.h"
#include "Filterable_ABC.h"
#include "StandardModel.h"

#include "clients_kernel/ModesObserver_ABC.h"
#include <QtGui/qtreeview.h>
#include <boost/function.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  RichTreeView
    @brief  RichTreeView
*/
// Created: ABR 2012-08-10
// =============================================================================
class RichTreeView : public QTreeView
                   , public kernel::ModesObserver_ABC
                   , public DragAndDropObserver_ABC
                   , public Filterable_ABC
{
   Q_OBJECT

public:
    //! @name Types
    //@{
    typedef boost::function< bool( const QModelIndex&, const QModelIndex&, bool& ) > T_LessThanFunctor;
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit RichTreeView( const QString& objectName, QWidget* parent = 0, kernel::Controllers* controllers = 0 );
    virtual ~RichTreeView();
    //@}

    //! @name Filterable_ABC implementation
    //@{
    virtual void ApplyFilters( const std::map< int, std::vector< Filter_ABC* > >& filters );
    virtual void CreateFilters( RichView_ABC& richView );
    virtual void Purge();
    virtual QHeaderView* GetHeader() const;
    //@}

    //! @name Operations
    //@{
    void EnableDragAndDrop( bool enable );
    virtual void Drop( const QString& /*mimeType*/, void* /*data*/, QStandardItem& /*target*/ ) {}
    virtual QMimeData* MimeData( const QModelIndexList& /*indexes*/, bool& /*overriden*/ ) const { return 0; }
    //@}

public slots:
    //! @name Search
    //@{
    void LockDragAndDrop( bool lock );
    void SearchAndSelect( const QString& searchedText, Qt::MatchFlags compareFlag = Qt::MatchContains, Qt::ItemDataRole searchRole = Qt::ToolTipRole );
    void SearchAndSelectNext( Qt::MatchFlags compareFlag = Qt::MatchContains, Qt::ItemDataRole searchRole = Qt::ToolTipRole  );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void startDrag( Qt::DropActions supportedActions );
    virtual void SetLessThanFunctor( const T_LessThanFunctor& functor );
    //@}

protected:
    //! @name Member data
    //@{
    QSortFilterProxyModel* proxyModel_;
    StandardModel dataModel_;
    QString searchedText_;
    bool creationBlocked_;
    bool contextMenuBlocked_;
    Qt::DropAction dropAction_;
    //@}
};

} //! namespace gui

#endif // __RichTreeView_h_
