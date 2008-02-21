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
             HomePage( QWidgetStack* pages, const tools::GeneralConfig& config );
    virtual ~HomePage();
    //@}
};

#endif // __HomePage_h_
