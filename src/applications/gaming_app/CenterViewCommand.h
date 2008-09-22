// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CenterViewCommand_h_
#define __CenterViewCommand_h_

#include "gaming/CommandHandler_ABC.h"

namespace gui
{
    class LinkInterpreter_ABC;
}

class CommandHandler;

// =============================================================================
/** @class  CenterViewCommand
    @brief  Center view command
*/
// Created: AGE 2008-06-12
// =============================================================================
class CenterViewCommand : public CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CenterViewCommand( CommandHandler& handler, gui::LinkInterpreter_ABC& interpreter );
    virtual ~CenterViewCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CenterViewCommand( const CenterViewCommand& );            //!< Copy constructor
    CenterViewCommand& operator=( const CenterViewCommand& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    gui::LinkInterpreter_ABC& interpreter_;
    //@}
};

#endif // __CenterViewCommand_h_
