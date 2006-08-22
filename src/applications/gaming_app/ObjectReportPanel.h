// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectReportPanel_h_
#define __ObjectReportPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class FireResultListView;
class Explosions;

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public gui::InfoPanel_ABC
                        , public kernel::Observer_ABC
                        , public kernel::ElementObserver_ABC< Explosions >
                        , public kernel::SelectionObserver< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectReportPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ObjectReportPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectReportPanel( const ObjectReportPanel& );
    ObjectReportPanel& operator=( const ObjectReportPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Explosions& );
    virtual void NotifyDeleted( const kernel::Object_ABC& );
    virtual void NotifySelected( const kernel::Object_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    FireResultListView* reports_;
    //@}
};

#endif // __ObjectReportPanel_h_
