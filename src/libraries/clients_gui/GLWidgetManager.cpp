// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLWidgetManager.h"
#include "moc_GLWidgetManager.cpp"

#include "ContourLinesComputer.h"
#include "ContourLinesObserver.h"
#include "Elevation2dTexture.h"
#include "FileDialog.h"
#include "FrustumInfos.h"
#include "GLDockWidget.h"
#include "GLMainProxy.h"
#include "GLOptions.h"
#include "GL2D3DProxy.h"
#include "GLStackedWidget.h"
#include "GL2DWidget.h"
#include "Layer_ABC.h"
#include "SignalAdapter.h"
#include "Tools.h"
#include "WatershedTexture.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Settings.h"
#include "clients_kernel/StaticModel.h"
#include "tools/ExerciseConfig.h"

using namespace gui;

namespace
{
    const int nbMaxGLDock = 15;
    const QString mainViewName = "MainView";
    const QString viewName = "View_%1";
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager constructor
// Created: ABR 2014-06-06
// -----------------------------------------------------------------------------
GLWidgetManager::GLWidgetManager( QMainWindow& mainWindow,
                                  kernel::Controllers& controllers,
                                  const kernel::Profile_ABC& profile,
                                  const tools::ExerciseConfig& config,
                                  const kernel::StaticModel& staticModel,
                                  const kernel::EntityResolver_ABC& model,
                                  const std::shared_ptr< IconLayout >& iconLayout,
                                  EventStrategy_ABC& strategy,
                                  GLMainProxy& mainProxy,
                                  const std::shared_ptr< Lighting_ABC >& lighting )
    : QObject( &mainWindow )
    , mainWindow_( mainWindow )
    , controllers_( controllers )
    , config_( config )
    , staticModel_( staticModel )
    , iconLayout_( iconLayout )
    , strategy_( strategy )
    , mainProxy_( mainProxy )
    , contourLinesObserver_( new ContourLinesObserver() )
    , dockWidgetCount_( 0 )
    , displayTimer_( new QTimer( this ) )
    , loading_( false )
{
    setObjectName( "GLWidgetManager" );
    mainProxy_.AddActiveChangeObserver( this, [&]( const GLView_ABC::T_View& ) {
        ResetContourLinesObservers();
        mainProxy_.GetActiveOptions().GetContourLinesComputer()->SetContourLinesObserver( contourLinesObserver_ );
    } );
    auto proxy = std::make_shared< GL2D3DProxy >( mainProxy,
                                                  controllers_,
                                                  profile,
                                                  staticModel_,
                                                  model,
                                                  lighting,
                                                  tr( "Main view" ),
                                                  0 );
    mainWidget_ = CreateStackedWidget( proxy );
    mainWindow_.setCentralWidget( mainWidget_.get() );
    gui::connect( displayTimer_.get(), SIGNAL( timeout() ), [&]() {
        mainProxy_.UpdateGL();
        emit UpdateGL();
    } );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager destructor
// Created: ABR 2014-06-06
// -----------------------------------------------------------------------------
GLWidgetManager::~GLWidgetManager()
{
    Purge();
    RemoveStackedWidget( mainWidget_ );
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::Load
// Created: ABR 2014-06-06
// -----------------------------------------------------------------------------
void GLWidgetManager::Load( const DrawingTypes& drawingTypes,
                            const tools::Path& settingsDirectory )
{
    settingsDirectory_ = settingsDirectory / tools::Path::FromUnicode( tr( "config.ini" ).toStdWString() );
    mainWidget_->Load();
    mainProxy_.Load( drawingTypes );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::Purge
// Created: ABR 2014-06-06
// -----------------------------------------------------------------------------
void GLWidgetManager::Purge()
{
    dockWidgetCount_ = 0;
    mainProxy_.SetHoveredView( 0 );
    displayTimer_->stop();
    mainWidget_->Close();
    PurgeViews();
    mainProxy_.Purge();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::PurgeViews
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void GLWidgetManager::PurgeViews()
{
    for( auto it = dockWidgets_.begin(); it != dockWidgets_.end(); ++it )
        RemoveStackedWidget( it->second->GetStackedWidget() );
    dockWidgets_.clear();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::GetStackedWidget
// Created: ABR 2014-07-18
// -----------------------------------------------------------------------------
GLWidgetManager::T_GLStackedWidget GLWidgetManager::GetStackedWidget( const std::function< bool( const T_GLStackedWidget& ) >& functor,
                                                                      const std::string& error ) const
{
    if( functor( mainWidget_ ) )
        return mainWidget_;
    auto it = std::find_if( dockWidgets_.begin(),
                            dockWidgets_.end(),
                            [=]( const std::pair< unsigned, T_GLDockWidget >& dock ) {
                                return dock.second->GetStackedWidget() && functor( dock.second->GetStackedWidget() );
                            } );
    if( it == dockWidgets_.end() )
        throw MASA_EXCEPTION( error.c_str() );
    return it->second->GetStackedWidget();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::OptionChanged
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
void GLWidgetManager::OptionChanged( const std::string& name,
                                     const kernel::OptionVariant& value )
{
    if( !controllers_.options_.GetGeneralOptions()->Has( name ) )
        mainProxy_.GetActiveOptions().Set( name, value );
    if( name == "RefreshRate" )
        displayTimer_->start( value.To< int >() );
    else if( name == "3D" )
    {
        auto activeWidget = GetActiveWidget();
        if( activeWidget && activeWidget->currentIndex() != GLStackedWidget::eWidget_Empty )
            activeWidget->ChangeTo( value.To< bool >() ? GLStackedWidget::eWidget_3D : GLStackedWidget::eWidget_2D );
    }
    else if( name == "ContourLines/Height" )
        SetContourLinesComputer( value.To< int >(), mainProxy_.GetActiveOptions() );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::SetContourLinesComputer
// Created: ABR 2014-07-17
// -----------------------------------------------------------------------------
void GLWidgetManager::SetContourLinesComputer( int height, GLOptions& options )
{
    auto computer = mainProxy_.GetContourLinesComputer( height );
    if( !computer )
    {
        computer = std::make_shared< ContourLinesComputer >( controllers_.controller_, staticModel_.detection_ );
        computer->SetHeight( height );
    }
    options.SetContourLinesComputer( computer );
    ResetContourLinesObservers();
    computer->SetContourLinesObserver( contourLinesObserver_ );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::NotifyUpdated
// Created: ABR 2014-06-26
// -----------------------------------------------------------------------------
void GLWidgetManager::NotifyUpdated( const kernel::Filter_ABC& filter )
{
    mainProxy_.GetActiveOptions().SetFilterEntity( filter.GetFilteredEntity() );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::AddView
// Created: ABR 2014-06-06
// -----------------------------------------------------------------------------
GLWidgetManager::T_GLStackedWidget GLWidgetManager::CreateStackedWidget( const std::shared_ptr< GLView_ABC >& proxy ) const
{
    auto view = std::make_shared< GLStackedWidget >( &mainWindow_,
                                                     proxy,
                                                     config_,
                                                     staticModel_.detection_,
                                                     strategy_,
                                                     iconLayout_,
                                                     mainWidget_ ? mainWidget_->GetWidget2d().get() : nullptr );
    mainProxy_.Register( proxy );
    connect( view.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ), SIGNAL( MouseMove( const geometry::Point2f& ) ) );
    connect( view.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ), SIGNAL( MouseMove( const geometry::Point3f& ) ) );
    return view;
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::AddView
// Created: ABR 2014-06-11
// -----------------------------------------------------------------------------
GLDockWidget* GLWidgetManager::AddDockWidget( unsigned id /* = 0 */ )
{
    if( controllers_.modes_.GetCurrentMode() <= eModes_Default )
        return 0;
    const bool wasFromMenu = id == 0;
    QString warning;
    if( wasFromMenu ) // from menu clic / F9 shortcut
    {
        for( unsigned i = 1; id == 0 && i <= nbMaxGLDock; ++i )
            if( dockWidgets_.count( i ) == 0 )
                id = i;
        if( id == 0 )
            warning = tr( "Unable to create a new terrain view, no identifier available." );
    }
    else if( id > nbMaxGLDock ) // from configuration file
        warning = tr( "Unable to create a new terrain view with an invalid identifier: %1." ).arg( id );
    else if( dockWidgets_.count( id ) != 0 )
        warning = tr( "Unable to create a new terrain view, identifier already in use: %1." ).arg( id );
    if( !warning.isEmpty() )
    {
        QMessageBox::warning( &mainWindow_, tr( "Warning" ), warning );
        return 0;
    }
    const auto& activeView = mainProxy_.GetActiveView();
    auto proxy = std::make_shared< GL2D3DProxy >( mainProxy_,
                                                  activeView,
                                                  tr( "View %1" ).arg( id ),
                                                  id );
    auto stackedWidget = CreateStackedWidget( proxy );
    stackedWidget->Load();
    proxy->LoadFrustum( activeView.SaveFrustum() );
    dockWidgets_[ id ] = std::make_shared< GLDockWidget >( controllers_, mainWindow_, stackedWidget );
    auto dockWidget = dockWidgets_[ id ].get();
    connect( dockWidget, SIGNAL( OnClose( const QWidget& ) ), SLOT( OnDockWidgetClosed( const QWidget& ) ) );
    if( wasFromMenu )
        mainProxy_.SetActiveView( *proxy );
    return dockWidget;
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::RemoveView
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
void GLWidgetManager::RemoveStackedWidget( const T_GLStackedWidget& view )
{
    if( !view )
        throw MASA_EXCEPTION( "Unable to remove an invalid GLView" );
    mainProxy_.Unregister( view->GetProxy() );
    view->Close();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::OnDockViewClosed
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
void GLWidgetManager::OnDockWidgetClosed( const QWidget& widget )
{
    auto it = std::find_if( dockWidgets_.begin(),
                            dockWidgets_.end(),
                            [&widget]( const std::pair< unsigned, T_GLDockWidget >& dockView ) {
                                return dockView.second.get() == &widget;
                            } );
    if( it == dockWidgets_.end() )
        throw MASA_EXCEPTION( "Unable to delete a GLDockWidget not registered" );
    RemoveStackedWidget( it->second->GetStackedWidget() );
    dockWidgets_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::GetMainView
// Created: ABR 2014-06-17
// -----------------------------------------------------------------------------
GLWidgetManager::T_GLStackedWidget GLWidgetManager::GetMainWidget() const
{
    return mainWidget_;
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::GetHoveredWidget
// Created: ABR 2014-06-16
// -----------------------------------------------------------------------------
GLWidgetManager::T_GLStackedWidget GLWidgetManager::GetHoveredWidget() const
{
    if( auto view = mainProxy_.GetHoveredView() )
        return GetStackedWidget([ &]( const T_GLStackedWidget& widget ) {
            return widget->GetProxy() == view;
        }, "Unable to find the hovered view" );
    return T_GLStackedWidget();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::GetActiveWidget
// Created: ABR 2014-06-16
// -----------------------------------------------------------------------------
GLWidgetManager::T_GLStackedWidget GLWidgetManager::GetActiveWidget() const
{
    return GetStackedWidget( [ &]( const T_GLStackedWidget& widget ) {
        return widget->GetProxy().get() == &mainProxy_.GetActiveView();
    }, "Unable to find the active view" );
}

namespace
{
    // todo: see if we can split state and geometry so we can use this for
    // registry loading as well
    void SaveGeometryAndState( QMainWindow& mainWindow,
                               QSettings& settings,
                               const QString& geometry,
                               const QString& state )
    {
        // Full-screen is well handled, but maximized state is bugged, so we force
        // qt window state to the actual windows window state, to be sure saveGeometry
        // records the good value
        if( !mainWindow.isFullScreen() )
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            bool maximized = false;
            if( GetWindowPlacement( GetActiveWindow(), &wp ) )
                maximized = wp.showCmd == SW_MAXIMIZE;
            if( maximized )
                mainWindow.showMaximized();
            else
                mainWindow.showNormal();
        }
        settings.beginGroup( "Geometry" );
        settings.setValue( geometry, mainWindow.saveGeometry() );
        settings.setValue( state, mainWindow.saveState() );
        settings.endGroup();
    }
    void RestoreGeometryAndState( QMainWindow& mainWindow,
                                  QSettings& settings,
                                  const QString& geometry,
                                  const QString& state )
    {
        // We force qt window state to normal before loading the window geometry, then
        // we manually restore the good state after having loaded the geometry.
        mainWindow.showNormal();
        settings.beginGroup( "Geometry" );
        mainWindow.restoreGeometry( settings.value( geometry ).toByteArray() );
        mainWindow.restoreState( settings.value( state ).toByteArray() );
        settings.endGroup();
        if( mainWindow.windowState() & Qt::WindowFullScreen )
            mainWindow.showFullScreen();
        else if( mainWindow.windowState() & Qt::WindowMaximized )
            mainWindow.showMaximized();
        else
            mainWindow.showNormal();
    }
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::OnSaveDisplaySettings
// Created: ABR 2014-07-15
// -----------------------------------------------------------------------------
void GLWidgetManager::OnSaveDisplaySettings()
{
    auto filename = FileDialog::getSaveFileName( &mainWindow_,
                                                 tr( "Save configuration" ),
                                                 settingsDirectory_,
                                                 tr( "Configuration file (*.ini)" ) );
    if( filename.IsEmpty() )
        return;
    {
        kernel::Settings settings( filename );
        settings.clear();
    }
    kernel::Settings settings( filename );
    settings.beginGroup( "Context" );
    settings.setValue( "Application", QFileInfo( QCoreApplication::applicationFilePath() ).fileName() );
    settings.setValue( "PhysicalBase", QString::fromStdWString( config_.GetPhysicalBase().ToUnicode() ) );
    settings.setValue( "ExerciseName", QString::fromStdWString( config_.GetExerciseName().BaseName().ToUnicode() ) );
    settings.endGroup();

    settings.beginGroup( "General" );
    controllers_.options_.GetGeneralOptions()->Save( settings );
    settings.endGroup();

    SaveGeometryAndState( mainWindow_, settings, "WindowGeometry", "WindowState" );
    SaveView( settings, mainWidget_ );
    for( auto it = dockWidgets_.begin(); it != dockWidgets_.end(); ++it )
        if( auto stackedWidget = it->second->GetStackedWidget() )
            SaveView( settings,  stackedWidget );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::OnLoadDisplaySettings
// Created: ABR 2014-07-15
// -----------------------------------------------------------------------------
void GLWidgetManager::OnLoadDisplaySettings()
{
    auto filename = FileDialog::getOpenFileName( &mainWindow_,
        tr( "Load configuration" ), settingsDirectory_, tr( "Configuration file (*.ini)" ) );
    if( filename.IsEmpty() )
        return;
    LoadDisplaySettings( filename );
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::LoadDisplaySettings
// Created: ABR 2014-07-15
// -----------------------------------------------------------------------------
void GLWidgetManager::LoadDisplaySettings( const tools::Path& filename )
{
    kernel::Settings settings( filename );
    settings.beginGroup( "Context" );
    const QString fileApplication = settings.value( "Application" ).toString();
    const QString filePhysicalBase = settings.value( "PhysicalBase" ).toString();
    const QString fileExercise = settings.value( "ExerciseName" ).toString();
    settings.endGroup();
    if( fileApplication != QFileInfo( QCoreApplication::applicationFilePath() ).fileName() ||
        filePhysicalBase != QString::fromStdWString( config_.GetPhysicalBase().ToUnicode() ) ||
        fileExercise != QString::fromStdWString( config_.GetExerciseName().BaseName().ToUnicode() ) )
    {
        QMessageBox::warning( &mainWindow_,
                              tr( "Warning" ),
                              tr( "Unable to load configuration file '%1'.\nThese settings are defined for the application '%2', on the base '%3' and the exercise '%4'." )
                              .arg( QString::fromStdWString( filename.ToUnicode() ) )
                              .arg( fileApplication )
                              .arg( filePhysicalBase )
                              .arg( fileExercise ) );
        return;
    }
    loading_ = true;
    PurgeViews();

    settings.beginGroup( "General" );
    controllers_.options_.GetGeneralOptions()->Load( settings );
    settings.endGroup();

    const auto children = settings.childGroups();
    for( auto it = children.begin(); it != children.end(); ++it )
    {
        const auto& child = *it;
        const int id = settings.value( child + "/ViewID", -1 ).toInt();
        if( id != -1 && viewName.arg( id ) == child )
            if( id == 0 )
                LoadView( settings, mainWidget_ );
            else if( auto* dockWidget = AddDockWidget( id ) )
                LoadView( settings, dockWidget->GetStackedWidget() );
    }
    controllers_.options_.UpdateViewOptions();
    RestoreGeometryAndState( mainWindow_, settings, "WindowGeometry", "WindowState" );
    loading_ = false;
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::SaveView
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void GLWidgetManager::SaveView( kernel::Settings& settings,
                                const T_GLStackedWidget& stackedWidget,
                                const QString& group /* = "" */ )
{
    if( !stackedWidget )
        return;
    auto view = stackedWidget->GetProxy();
    settings.beginGroup( group.isEmpty() ? viewName.arg( view->GetID() ) : group );
    settings.setValue( "ViewID", view->GetID() );
    settings.beginGroup( "Situation" );
    view->SaveFrustum().Save( settings );
    settings.endGroup();
    view->GetActiveOptions().Save( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::LoadView
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void GLWidgetManager::LoadView( kernel::Settings& settings,
                                const T_GLStackedWidget& stackedWidget,
                                const QString& group /* = "" */ )
{
    if( !stackedWidget )
        return;
    auto view = stackedWidget->GetProxy();
    auto& options = view->GetActiveOptions();
    settings.beginGroup( group.isEmpty() ? viewName.arg( view->GetID() ) : group );
    options.Load( settings );
    stackedWidget->ChangeTo( options.Get( "3D" ).To< bool >() ? GLStackedWidget::eWidget_3D : GLStackedWidget::eWidget_2D );
    SetContourLinesComputer( options.Get( "ContourLines/Height" ).To< int >(), options );
    view->UpdateLayerOrder();
    settings.beginGroup( "Situation" );
    view->LoadFrustum( FrustumInfos( settings ) );
    settings.endGroup();
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::IsLoading
// Created: ABR 2014-07-18
// -----------------------------------------------------------------------------
bool GLWidgetManager::IsLoading() const
{
    return loading_;
}

// -----------------------------------------------------------------------------
// Name: GLWidgetManager::ResetContourLinesObservers
// Created: ABR 2014-08-06
// -----------------------------------------------------------------------------
void GLWidgetManager::ResetContourLinesObservers()
{
    mainProxy_.ApplyToOptions( []( GLOptions& options ) {
        options.GetContourLinesComputer()->SetContourLinesObserver( std::shared_ptr< ContourLinesObserver >() );
    } );
}

void GLWidgetManager::NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& menu )
{
    if( mainWidget_ != GetActiveWidget() )
        menu.InsertItem( "Interface", tr( "Use as main view" ), this, SLOT( OnChangeMainView() ) );
}

void GLWidgetManager::OnChangeMainView()
{
    auto activeWidget = GetActiveWidget();
    kernel::Settings settings;
    settings.remove( "TMP" );
    SaveView( settings, activeWidget, "TMP" );
    LoadView( settings, mainWidget_, "TMP" );
    settings.remove( "TMP" );
}
