// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandFacade_h_
#define __CommandFacade_h_

#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class LinkInterpreter_ABC;
    class GLView_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class CommandHandler_ABC;
class CommandHandler;
class QMainWindow;
class ProfileFilter;

// =============================================================================
/** @class  CommandFacade
    @brief  CommandFacade
*/
// Created: AGE 2008-06-12
// =============================================================================
class CommandFacade : public QObject
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandFacade( QMainWindow* parent, kernel::Controllers& controllers, tools::ExerciseConfig& config,
                            CommandHandler& handler, gui::LinkInterpreter_ABC& interpreter, gui::GLView_ABC& view,
                            ProfileFilter& profile, const kernel::CoordinateConverter_ABC& converter );
    virtual ~CommandFacade();
    //@}

private:
    //! @name Helpers
    //@{
    void Add( CommandHandler_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_vector< CommandHandler_ABC > handlers_;
    //@}
};

#endif // __CommandFacade_h_
