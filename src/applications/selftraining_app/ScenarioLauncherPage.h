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

namespace tools
{
    class Loader_ABC;
}

class Application;
class ExerciseContainer;
class ExerciseList;
class ProgressPage;
class DebugConfigPanel;

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
             ScenarioLauncherPage( Application& app, QStackedWidget* pages,
                                   Page_ABC& previous, kernel::Controllers& controllers,
                                   const Config& config,
                                   const tools::Loader_ABC& fileLoader,
                                   ExerciseContainer& exercises );
    virtual ~ScenarioLauncherPage();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnStart();
    void OnSelect( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void OnSelectCheckpoint( const tools::Path& session, const tools::Path& checkpoint );
    void OnClientEnabled( bool enabled );
    void OnIntegrationPathSelected( const tools::Path& integrationDir );
    void OnDumpPathfindOptionsChanged( const QString& filter, const tools::Path& directory );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    virtual void Update();

    void CreateSession( const tools::Path& exercise, const tools::Path& session ) const;
    std::pair< tools::Path, bool > BuildSessionName() const;
    template< typename T >
    T* AddPlugin( T* plugin );
    bool CanBeStarted() const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< frontend::PluginConfig_ABC* > T_Plugins;
    //@}

private:
    //! @name Member data
    //@{
    const Config&                 config_;
    const tools::Loader_ABC&      fileLoader_;
    kernel::Controllers&          controllers_;
    ExerciseContainer&            exerciseContainer_;
    ProgressPage*                 progressPage_;
    ExerciseList*                 exercises_;
    const frontend::Exercise_ABC* exercise_;
    frontend::Profile             profile_;
    tools::Path                   session_;
    tools::Path                   checkpoint_;
    tools::Path                   integrationDir_;
    QString                       pathfindFilter_;
    tools::Path                   dumpPathfindDirectory_;
    T_Plugins                     plugins_;
    QTabWidget*                   tabs_;
    QTabWidget*                   configTabs_;
    bool                          hasClient_;
    DebugConfigPanel*             configPanel_;
    //@}
};

#endif // __ScenarioLauncherPage_h_
