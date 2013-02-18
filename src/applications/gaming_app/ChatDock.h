// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChatDock_h_
#define __ChatDock_h_

#include "clients_gui/RichDockWidget.h"

namespace kernel
{
    class Controllers;
}

class Publisher_ABC;
class CommandHandler;

// =============================================================================
/** @class  ChatDock
    @brief  ChatDock
*/
// Created: SBO 2008-06-11
// =============================================================================
class ChatDock : public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             ChatDock( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~ChatDock();
    //@}
};

#endif // __ChatDock_h_
