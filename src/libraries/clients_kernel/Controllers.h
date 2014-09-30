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
#include "ENT/ENT_Enums.h"

namespace tools
{
    class  Observer_ABC;
}

namespace kernel
{
    class ActionController;
    class Controller;
    class LanguageController;
    class ModeController;
    class OptionsController;

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
    E_Modes GetCurrentMode() const;
    void ChangeMode( E_Modes newMode );
    void LoadOptions( E_Modes mode );
    void SaveOptions( E_Modes mode );
    //@}

public:
    //! @name Member data
    //@{
    OptionsController&  options_;
    Controller&         controller_;
    ActionController&   actions_;
    ActionController&   eventActions_;
    ModeController&     modes_;
    LanguageController& languages_;
    //@}
};

} //! namespace kernel

#endif // __Controllers_h_
