// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DockCommand_h_
#define __DockCommand_h_

#include "gaming/CommandHandler_ABC.h"

class CommandHandler;
class QDockWidget;
class QMainWindow;

// =============================================================================
/** @class  DockCommand
    @brief  DockCommand
*/
// Created: SBO 2008-07-02
// =============================================================================
class DockCommand : public CommandHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DockCommand( CommandHandler& handler, QMainWindow& mainWindow );
    virtual ~DockCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DockCommand( const DockCommand& );            //!< Copy constructor
    DockCommand& operator=( const DockCommand& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    QMainWindow& mainWindow_;
    //@}
};

#endif // __DockCommand_h_
