// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScenarioPage_h_
#define __ScenarioPage_h_

#include "MenuPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class NetworkExerciseLister;

// =============================================================================
/** @class  ScenarioPage
    @brief  ScenarioPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ScenarioPage : public MenuPage
{

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, NetworkExerciseLister& lister );
    virtual ~ScenarioPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioPage( const ScenarioPage& );            //!< Copy constructor
    ScenarioPage& operator=( const ScenarioPage& ); //!< Assignment operator
    //@}
};

#endif // __ScenarioPage_h_
