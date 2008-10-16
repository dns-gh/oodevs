// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ScenarioLauncherPage_h_
#define __ScenarioLauncherPage_h_

#include "ContentPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class ExerciseList;
class ScenarioLauncherPageOptions;

// =============================================================================
/** @class  ScenarioLauncherPage
    @brief  ScenarioLauncherPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ScenarioLauncherPage : public ContentPage
{

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config );
    virtual ~ScenarioLauncherPage();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioLauncherPage( const ScenarioLauncherPage& );            //!< Copy constructor
    ScenarioLauncherPage& operator=( const ScenarioLauncherPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    //@}

private:
    //! @name Member data
    //@{
    ExerciseList* exercises_;
    //@}
};

#endif // __ScenarioLauncherPage_h_
