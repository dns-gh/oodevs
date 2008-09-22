// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ZoomViewCommand_h_
#define __ZoomViewCommand_h_

#include "gaming/CommandHandler_ABC.h"

namespace gui
{
    class View_ABC;
}

class CommandHandler;

// =============================================================================
/** @class  ZoomViewCommand
    @brief  ZoomViewCommand
*/
// Created: SBO 2008-07-07
// =============================================================================
class ZoomViewCommand : public CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ZoomViewCommand( CommandHandler& handler, gui::View_ABC& view );
    virtual ~ZoomViewCommand();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ZoomViewCommand( const ZoomViewCommand& );            //!< Copy constructor
    ZoomViewCommand& operator=( const ZoomViewCommand& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Center( const std::string& arg );
    //@}

private:
    //! @name Member data
    //@{
    CommandHandler& handler_;
    gui::View_ABC& view_;
    //@}
};

#endif // __ZoomViewCommand_h_
