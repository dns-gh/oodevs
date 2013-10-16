// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignsWidget_ABC_h_
#define __LogisticConsignsWidget_ABC_h_

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include "clients_kernel/Tools.h"
#include <tools/SelectionObserver_ABC.h>

// =============================================================================
/** @class  __LogisticConsignsWidget_ABC_h_
    @brief  __LogisticConsignsWidget_ABC_h_
*/
// Created: MMC 2012-10-29
// =============================================================================
class LogisticConsignsWidget_ABC : public QWidget
                                 , public tools::Observer_ABC
                                 , public tools::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignsWidget_ABC( QWidget* parent, kernel::DisplayExtractor_ABC& extractor );
    virtual ~LogisticConsignsWidget_ABC();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    void OnHandledLinkClicked( const QString& url, const QModelIndex& index );
    void OnItemExpanded( QTreeWidgetItem* item );
    //@}

protected:
    //! @name Member data
    //@{
    QTreeWidget* pConsignTreeView_;
    QTreeWidget* pConsignHandledTreeView_;
    kernel::DisplayExtractor_ABC& extractor_;
    //@}

public:
    //! @name Operations
    //@{
    void SetTreeWidgetItemData( QTreeWidgetItem& item, const QString& key, const QString& value );
    QTreeWidgetItem* AddTreeWidgetItem( QTreeWidgetItem& parent, const QString& key, const QString& value );
    //@}

private:
    //! @name Helpers
    //@{
    QTreeWidget* CreateTreeWidget( const QString& title );
    //@}
};

#endif // __LogisticConsignsWidget_ABC_h_
