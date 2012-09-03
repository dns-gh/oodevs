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
#include "StandardModel.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class SearchListView_ABC;

// =============================================================================
/** @class  RichTreeView
    @brief  RichTreeView
*/
// Created: ABR 2012-08-10
// =============================================================================
class RichTreeView : public QTreeView
                   , public kernel::ReadOnlyModable
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
    //@}

    //! @name Locks
    //@{
    void SetCreationBlocked( bool creationBlocked );
    bool IsCreationBlocked() const;
    void SetContextMenuBlocked( bool contextMenuBlocked );
    bool IsContextMenuBlocked() const;
    void SetDragAndDropBlocked( bool dragAndDropBlocked );
    bool IsDragAndDropBlocked() const;
    //@}

    //! @name Filters
    //@{
    virtual void CreateFilters( SearchListView_ABC& searchListView );
    //@}

public slots:
    //! @name Search
    //@{
    void SearchAndSelect( const QString& searchedText );
    void SearchAndSelectNext();
    //@}

protected:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Search helpers
    //@{
    bool MatchItem() const;
    //@}

protected:
    //! @name Member data
    //@{
    QSortFilterProxyModel   proxyModel_;
    StandardModel           dataModel_;
    QString                 searchedText_;
    bool                    creationBlocked_;
    bool                    contextMenuBlocked_;
    bool                    dndBlocked_;
    //@}
};

} //! namespace gui

#endif // __RichTreeView_h_
