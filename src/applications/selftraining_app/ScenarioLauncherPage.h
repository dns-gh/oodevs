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

namespace frontend
{
    class SpawnCommand;
    class InfoBubble;
}

namespace kernel
{
    class Controllers;
}

class Session;
class ExerciseList;
class ScenarioLauncherPageOptions;
class SessionRunningPage;

// =============================================================================
/** @class  ScenarioLauncherPage
    @brief  ScenarioLauncherPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ScenarioLauncherPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, SessionRunningPage& running, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus  );
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
    void CreateSession( const QString& exercise, const QString& session );
    void StartSession( Session* session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&  config_;
    boost::shared_ptr< Session > sessionStatus_;
    ExerciseList*                exercises_;
    ScenarioLauncherPageOptions* options_;
    //@}
};

#endif // __ScenarioLauncherPage_h_
