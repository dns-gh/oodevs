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
class ChatDock : public QDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             ChatDock( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~ChatDock();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ChatDock( const ChatDock& );            //!< Copy constructor
    ChatDock& operator=( const ChatDock& ); //!< Assignment operator
    //@}
};

#endif // __ChatDock_h_
