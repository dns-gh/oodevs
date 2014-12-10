// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineDockWidget_h_
#define __TimelineDockWidget_h_

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/shared_ptr.hpp>

namespace actions
{
    class Action_ABC;
}

namespace gui
{
    class Event;
    class GLWidgetManager;
    class VisibilityFilter_ABC;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ModelUnLoaded;
    class Profile_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class GamingConfig;
class Model;
class Profile;
class TimelineWebView;
class TimelineToolBar;

// =============================================================================
/** @class  TimelineDockWidget
    @brief  TimelineDockWidget
*/
// Created: ABR 2013-05-14
// =============================================================================
class TimelineDockWidget : public gui::RichDockWidget
                         , public tools::ElementObserver_ABC< gui::VisibilityFilter_ABC >
                         , public tools::ElementObserver_ABC< gui::Event >
                         , public tools::ElementObserver_ABC< Profile >
                         , public tools::SelectionObserver_ABC
                         , public tools::SelectionObserver_Base< kernel::Entity_ABC >
                         , public tools::SelectionObserver_Base< actions::Action_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const GamingConfig& config, Model& model, gui::GLWidgetManager& glWidgetManager, const kernel::Profile_ABC& profile );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    const boost::shared_ptr< TimelineWebView >& GetWebView() const;
    void Purge();
    //@}

    //! @name ElementObserver_ABC implementation
    //@{
    virtual void NotifyCreated( const gui::VisibilityFilter_ABC& filter );
    virtual void NotifyUpdated( const gui::VisibilityFilter_ABC& filter );
    virtual void NotifyUpdated( const gui::Event& event );
    virtual void NotifyUpdated( const Profile& profile );
    virtual void BeforeSelection();
    virtual void Select( const kernel::Entity_ABC& element );
    virtual void Select( const actions::Action_ABC& action );
    virtual void AfterSelection();
    //@}

public slots:
    //! @name Slots
    //@{
    TimelineToolBar* AddView( bool main = false, const std::string& name = "" );
    void RemoveCurrentView();
    void OnCurrentChanged( int index );
    void OnTabContextMenu();
    void OnRenameTab();
    void OnLoadRequested();
    void OnShowOnlyFilterChanged( const std::string& uuid, const std::string& name );
    void OnSelectedFilterChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateWebView( TimelineToolBar& toolbar ) const;
    std::string GetEntityFilter( const TimelineToolBar& toolbar ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    const std::string gamingUuid_;
    QTabWidget* tabWidget_;
    QMenu* contextMenu_;
    TimelineToolBar* mainView_;
    boost::shared_ptr< TimelineWebView > webView_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    // map of < event's uuid, tab's widget >, so we can retrieve the right tab even if the event's name has changed.
    std::map< std::string, QWidget* > showOnlyViews_;
    //@}

private:
    //! @name Static member data
    //@{
    static int maxTabNumber_;
    //@}
};

#endif // __TimelineDockWidget_h_
