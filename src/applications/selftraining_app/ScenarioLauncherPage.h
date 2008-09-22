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

class QListBox;
class QTextBrowser;
class QTabWidget; 
class QLineEdit ; 

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class SpawnCommand ; 
    class InfoBubble ; 
}

class SessionStatus ; 

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
             ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, SessionRunningPage& running, const tools::GeneralConfig& config, boost::shared_ptr< SessionStatus > sessionStatus  );
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
    //@}

private:
    
    boost::shared_ptr< SessionStatus > sessionStatus_ ; 

    //! @name Helpers
    //@{
    void StartSession( SessionStatus* session ) ; 
    //@}

    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    ExerciseList*               exercises_ ; 
    ScenarioLauncherPageOptions*        options_ ; 
    //@}
};

#endif // __ScenarioLauncherPage_h_
