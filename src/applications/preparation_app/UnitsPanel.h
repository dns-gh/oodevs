// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitsPanel_h_
#define __UnitsPanel_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class AgentTypes;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  UnitsPanel
    @brief  UnitsPanel
*/
// Created: SBO 2006-08-28
// =============================================================================
class UnitsPanel : public gui::InfoPanel_ABC
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             UnitsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::AgentTypes& agentTypes, gui::ItemFactory_ABC& factory );
    virtual ~UnitsPanel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UnitsPanel( const UnitsPanel& );            //!< Copy constructor
    UnitsPanel& operator=( const UnitsPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Data members
    //@{
    kernel::Controllers& controllers_;
    gui::ValuedComboBox< std::string >* combo_;
    //@}
};

#endif // __UnitsPanel_h_
