// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMissionsTab_h_
#define __InfoMissionsTab_h_

#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace gui
{
    class DisplayExtractor;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace actions
{
    class Action_ABC;
    class Parameter_ABC;
}

class MissionParameters;

// =============================================================================
/** @class  InfoMissionsTab
    @brief  InfoMissionsTab
*/
// Created: SBO 2007-04-18
// =============================================================================
class InfoMissionsTab : public QTreeView
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< MissionParameters >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InfoMissionsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor );
    virtual ~InfoMissionsTab();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const MissionParameters& extension );
    template< typename T >
    void RecursiveDisplay( const T& element, QStandardItem* item );
    void DisplayParameter( const actions::Parameter_ABC& param, QStandardItem* item1, QStandardItem* item2 );
    bool ShouldUpdate( const MissionParameters& extension ) const;
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QTabWidget* parent_;
    QStandardItemModel missionModel_;
    QSortFilterProxyModel* proxyFilter_;
    QPixmap error_;
    //@}
};

#endif // __InfoMissionsTab_h_
