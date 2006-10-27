// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentStatePanel_h_
#define __AgentStatePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class PropertiesWidget;
}

// =============================================================================
/** @class  AgentStatePanel
    @brief  AgentStatePanel
*/
// Created: SBO 2006-10-11
// =============================================================================
class AgentStatePanel : public gui::InfoPanel_ABC
                      , public kernel::Observer_ABC
                      , public kernel::SelectionObserver< kernel::Agent_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentStatePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::EditorFactory_ABC& editorFactory );
    virtual ~AgentStatePanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Agent_ABC* element );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentStatePanel( const AgentStatePanel& );            //!< Copy constructor
    AgentStatePanel& operator=( const AgentStatePanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyDeleted( const kernel::Agent_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesWidget* properties_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

#endif // __AgentStatePanel_h_
