// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __gui_PropertiesPanel_h_
#define __gui_PropertiesPanel_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class GlProxy;
    class PropertiesWidget;
    class TableItemDisplayer;

// =============================================================================
/** @class  PropertiesPanel
    @brief  Panel to display Entity properties registered into
            PropertiesDictionary extension
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesPanel : public QScrollArea
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory, gui::TableItemDisplayer& displayer, const GlProxy& glProxy );
    virtual ~PropertiesPanel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&        controllers_;
    const GlProxy&              glProxy_;
    gui::PropertiesWidget*      table_;
    const kernel::Entity_ABC*   selected_;
    gui::TableItemDisplayer&    displayer_;
    //@}
};

}

#endif // __gui_PropertiesPanel_h_
