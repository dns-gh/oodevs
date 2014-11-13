// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef gui_GLWidgetManager_h
#define gui_GLWidgetManager_h

#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class EntityResolver_ABC;
    class Filter_ABC;
    class Profile_ABC;
    class StaticModel;
    class Settings;
}

namespace tools
{
    class ExerciseConfig;
    class Path;
}

class EventStrategy_ABC;
class Lighting_ABC;

namespace gui
{
    class ContourLinesObserver;
    class DrawingTypes;
    class GLDockWidget;
    class GLMainProxy;
    class GLOptions;
    class GLProxy_ABC;
    class GLStackedWidget;
    class GL2DWidget;
    class GLView_ABC;
    class IconLayout;
    class Layer_ABC;

// =============================================================================
/** @class  GLWidgetManager
    @brief  GLWidgetManager
*/
// Created: ABR 2014-06-06
// =============================================================================
class GLWidgetManager : public QObject
                      , public tools::Observer_ABC
                      , public kernel::OptionsObserver_ABC
                      , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    typedef std::shared_ptr< GLStackedWidget >                T_GLStackedWidget;
    typedef std::shared_ptr< GLDockWidget >                   T_GLDockWidget;
    typedef std::function< void( const T_GLStackedWidget& ) > T_GLObserver;
    typedef std::vector< T_GLObserver >                       T_GLObservers;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GLWidgetManager( QMainWindow& mainWindow,
                              kernel::Controllers& controllers,
                              const kernel::Profile_ABC& profile,
                              const tools::ExerciseConfig& config,
                              const kernel::StaticModel& staticModel,
                              const kernel::EntityResolver_ABC& model,
                              const std::shared_ptr< IconLayout >& iconLayout,
                              EventStrategy_ABC& strategy,
                              GLMainProxy& proxy,
                              const std::shared_ptr< Lighting_ABC >& lighting );
    virtual ~GLWidgetManager();
    //@}

    //! @name Operations
    //@{
    void Load( const DrawingTypes& drawingTypes,
               const tools::Path& settingsDirectory );
    void Purge();

    void LoadDisplaySettings( const tools::Path& filename );

    bool IsLoading() const;
    T_GLStackedWidget GetMainWidget() const;
    T_GLStackedWidget GetHoveredWidget() const;
    //@}

public slots:
    //! @name Slots
    //@{
    // if id == 0 then add next available id, else add given id.
    // if id < 0 || id > nbMaxGLDock -> warning, add nothing
    GLDockWidget* AddDockWidget( unsigned id = 0 );
    void OnDockWidgetClosed( const QWidget& widget );
    void OnSaveDisplaySettings();
    void OnLoadDisplaySettings();
    //@}

signals:
    //! @name Signals
    //@{
    void MouseMove( const geometry::Point2f& );
    void MouseMove( const geometry::Point3f& );
    void UpdateGL();
    //@}

private:
    //! @name Observers implementation
    //@{
    virtual void OptionChanged( const std::string& name,
                                const kernel::OptionVariant& value );
    virtual void NotifyUpdated( const kernel::Filter_ABC& filter );
    //@}

    //! @name Helpers
    //@{
    T_GLStackedWidget CreateStackedWidget( const std::shared_ptr< GLView_ABC >& proxy ) const;
    void RemoveStackedWidget( const T_GLStackedWidget& view );
    void SaveView( kernel::Settings& settings, const T_GLStackedWidget& view );
    void LoadView( kernel::Settings& settings, const T_GLStackedWidget& view );
    void PurgeViews();
    void SetContourLinesComputer( int height, GLOptions& options );
    T_GLStackedWidget GetStackedWidget( const std::function< bool( const T_GLStackedWidget& ) >& functor,
                                        const std::string& error ) const;
    void ResetContourLinesObservers();
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow& mainWindow_;
    kernel::Controllers& controllers_;
    const tools::ExerciseConfig& config_;
    const kernel::StaticModel& staticModel_;
    std::shared_ptr< IconLayout > iconLayout_;
    EventStrategy_ABC& strategy_;
    GLMainProxy& mainProxy_;
    tools::Path settingsDirectory_;
    std::shared_ptr< ContourLinesObserver > contourLinesObserver_;

    int dockWidgetCount_;
    bool loading_;
    std::auto_ptr< QTimer > displayTimer_;
    T_GLStackedWidget mainWidget_;
    std::map< unsigned, T_GLDockWidget > dockWidgets_;
    //@}
};

} //! namespace gui

#endif // GLWidgetManager_h
