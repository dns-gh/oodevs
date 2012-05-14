// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Controllers_h_
#define __Controllers_h_

#include <boost/noncopyable.hpp>

class QMainWindow;

namespace tools
{
    class  Observer_ABC;
}

namespace kernel
{
    class ActionController;
    class Controller;
    class ModeController_ABC;
    class Options;

// =============================================================================
/** @class  Controllers
    @brief  Controllers
*/
// Created: AGE 2006-03-22
// =============================================================================
class Controllers : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Controllers();
    virtual ~Controllers();
    //@}

    //! @name Operations
    //@{
    void Register( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );
    void Update( tools::Observer_ABC& observer );
    //@}

    //! @name Setters
    //@{
    void SetModeController( ModeController_ABC* modeController );
    //@}

public:
    //! @name Member data
    //@{
    Options&            options_;
    Controller&         controller_;
    ActionController&   actions_;
    ModeController_ABC* modes_;
    //@}
};

} //! namespace kernel

#endif // __Controllers_h_
