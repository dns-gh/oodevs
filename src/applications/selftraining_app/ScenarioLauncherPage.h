// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ScenarioLauncherPage_h_
#define __ScenarioLauncherPage_h_

#include "LauncherClientPage.h"
#include "frontend/Profile.h"
#include <vector>

namespace frontend
{
    class Config;
    class Exercise_ABC;
    class PluginConfig_ABC;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class LinkInterpreter_ABC;
}

class ExerciseList;
class ProgressPage;
class QTabWidget;

// =============================================================================
/** @class  ScenarioLauncherPage
    @brief  ScenarioLauncherPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ScenarioLauncherPage : public LauncherClientPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers
                 , const frontend::Config& config, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter, const QString& title = "" );
    virtual ~ScenarioLauncherPage();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnStart();
    void OnSelect( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void OnSelectCheckpoint( const QString& session, const QString& checkpoint );
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
    virtual std::string BuildSessionName() const;
    void CreateSession( const QString& exercise, const QString& session );
    template< typename T >
    T* AddPlugin( QTabWidget* tabs, const QString& name );
    bool CanBeStarted() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< frontend::PluginConfig_ABC* > T_Plugins;
    //@}

private:
    //! @name Member data
    //@{
    const frontend::Config&       config_;
    kernel::Controllers&          controllers_;
    gui::LinkInterpreter_ABC&     interpreter_;
    ProgressPage*                 progressPage_;
    ExerciseList*                 exercises_;
    const frontend::Exercise_ABC* exercise_;
    frontend::Profile             profile_;
    QString                       session_;
    QString                       checkpoint_;
    T_Plugins                     plugins_;
    //@}
};

#endif // __ScenarioLauncherPage_h_
