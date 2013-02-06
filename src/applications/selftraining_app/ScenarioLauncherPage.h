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
#include "Config.h"
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

namespace tools
{
    class Loader_ABC;
}

class Application;
class ExerciseList;
class ProgressPage;

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
             ScenarioLauncherPage( Application& app, QStackedWidget* pages,
                                   Page_ABC& previous, kernel::Controllers& controllers,
                                   const Config& config,
                                   const tools::Loader_ABC& fileLoader,
                                   frontend::LauncherClient& launcher,
                                   gui::LinkInterpreter_ABC& interpreter );
    virtual ~ScenarioLauncherPage();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnStart();
    void OnSelect( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void OnSelectCheckpoint( const QString& session, const QString& checkpoint );
    void OnSwordVersionSelected( bool isLegacy );
    void OnNoClientSelected( bool noClient );
    void OnIntegrationPathSelected( const QString& integrationDir );
    void OnDumpPathfindOptionsChanged( const QString& filter, const QString& directory );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void Update();
    virtual std::string BuildSessionName() const;
    void CreateSession( const QString& exercise, const QString& session );
    template< typename T >
    T* AddPlugin();
    bool CanBeStarted() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< frontend::PluginConfig_ABC* > T_Plugins;
    //@}

protected:
    //! @name Member data
    //@{
    const Config&                 config_;
    const tools::Loader_ABC&      fileLoader_;
    kernel::Controllers&          controllers_;
    gui::LinkInterpreter_ABC&     interpreter_;
    ProgressPage*                 progressPage_;
    ExerciseList*                 exercises_;
    const frontend::Exercise_ABC* exercise_;
    frontend::Profile             profile_;
    QString                       session_;
    QString                       checkpoint_;
    QString                       integrationDir_;
    QString                       pathfindFilter_;
    QString                       dumpPathfindDirectory_;
    T_Plugins                     plugins_;
    QTabWidget*                   tabs_;
    QTabWidget*                   configTabs_;
    bool                          isLegacy_;
    bool                          noClient_;
    //@}
};

#endif // __ScenarioLauncherPage_h_
