// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __LauncherCommand_h_
#define __LauncherCommand_h_

#include "gaming/CommandHandler_ABC.h"

namespace gui
{
    class LinkInterpreter_ABC;
}

namespace tools
{
    class ExerciseConfig ; 
}

class CommandHandler;

// =============================================================================
/** @class  LauncherCommand
    @brief  LauncherCommand
*/
// Created: RDS 2008-09-01
// =============================================================================
class LauncherCommand : public CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LauncherCommand( CommandHandler& handler , tools::ExerciseConfig& config, gui::LinkInterpreter_ABC& interpreter );
    virtual ~LauncherCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherCommand( const LauncherCommand& );            //!< Copy constructor
    LauncherCommand& operator=( const LauncherCommand& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    gui::LinkInterpreter_ABC& interpreter_ ; 
    tools::ExerciseConfig& config_ ; 
    //@}
};

#endif // __LauncherCommand_h_
