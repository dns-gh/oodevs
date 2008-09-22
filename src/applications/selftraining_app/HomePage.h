// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HomePage_h_
#define __HomePage_h_

#include "MenuPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace gui 
{
    class LinkInterpreter_ABC ; 
}

class SessionStatus ; 

// =============================================================================
/** @class  HomePage
    @brief  HomePage
*/
// Created: SBO 2008-02-21
// =============================================================================
class HomePage : public MenuPage
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit HomePage( QWidgetStack* pages, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< SessionStatus > sessionStatus );
    virtual ~HomePage();
    //@}
};

#endif // __HomePage_h_
