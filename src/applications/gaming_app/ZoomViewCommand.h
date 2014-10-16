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

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class GLView_ABC;
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
             ZoomViewCommand( CommandHandler& handler, gui::GLView_ABC& view,
                              const kernel::CoordinateConverter_ABC& converter );
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
    gui::GLView_ABC& view_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __ZoomViewCommand_h_
