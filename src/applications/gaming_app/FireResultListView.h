// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultListView_h_
#define __FireResultListView_h_

#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace gui
{
    class DisplayExtractor;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Explosions;
class PopulationFireResult;
class AgentFireResult;
class Equipment;
class Casualties;

// =============================================================================
/** @class  FireResultListView
    @brief  FireResultListView
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultListView : public QTreeWidget
                         , public tools::Observer_ABC
                         , public tools::SelectionObserver< kernel::Entity_ABC >
                         , public tools::ElementObserver_ABC< Explosions >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             FireResultListView( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor );
    virtual ~FireResultListView();
    //@}

    //! @name Operations
    //@{
    void Display( const AgentFireResult& result, QTreeWidgetItem* item );
    void Display( const PopulationFireResult& result, QTreeWidgetItem* item );
    void Display( const Equipment& equipment, QTreeWidgetItem* item );
    void Display( const Casualties& casualties, QTreeWidgetItem* item );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Explosions& results );
    void DisplayFirer( QTreeWidgetItem* item, const kernel::Entity_ABC* firer );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __FireResultListView_h_
