// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineWebView_h_
#define __TimelineWebView_h_

#include "ENT/ENT_Enums.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/TimelineHandler_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace gui
{
    class Event;
}

namespace kernel
{
    class Controllers;
    class ContextMenu;
    class Profile_ABC;
}

namespace timeline
{
    struct Error;
    struct Event;
    typedef std::vector< Event > Events;
    class Server_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class GamingConfig;
class Model;

// =============================================================================
/** @class  TimelineWebView
    @brief  TimelineWebView
*/
// Created: ABR 2013-05-28
// =============================================================================
class TimelineWebView : public QWidget
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< QDateTime >
                      , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                      , public kernel::TimelineHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineWebView( QWidget* parent,
                              const GamingConfig& config,
                              kernel::Controllers& controllers,
                              Model& model );
    virtual ~TimelineWebView();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
    //@}

    //! @name Filters
    //@{
    void UpdateFilters( const std::string& unitFilter,
                        bool displayEngaged,
                        const std::string& services,
                        const std::string& keyword,
                        const std::string& hierarchies,
                        const std::string& showOnly );
    void OnShowOnlyFilterChanged( const std::string& uuid );
    void RenameEvent( const std::string& uuid, const std::string& name );
    //@}

private:
    //! @name Helpers
    //@{
    gui::Event& GetOrCreateEvent( const timeline::Event& event );

    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );
    virtual void NotifyCreated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );

    void SetProfile( const QString& profile );

    void ReadActions( xml::xisubstream xis, const tools::Path& filename );
    void ReadAction( timeline::Events& events, xml::xistream& xis, QStringList& errors );
    //@}

    //! @name TimelineHandler_ABC implementation
    //@{
    virtual void CreateEvent( const timeline::Event& event, bool select = false );
    virtual void SelectEvent( const std::string& uuid );
    virtual void EditEvent( const timeline::Event& event );
    virtual void DeleteEvent( const std::string& uuid );
    virtual const std::string& GetCurrentParent() const;
    //@}

signals:
    //! @name Signals
    //@{
    void StartCreation( E_EventTypes type, const QDateTime& dateTime );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCenterView();
    void OnToggleLayoutOrientation();

    void OnCreatedEvents( const timeline::Events& events, const timeline::Error& error );
    void OnEditedEvent( const timeline::Event& event, const timeline::Error& error );
    void OnDeletedEvents( const std::vector< std::string >& uuids, const timeline::Error& error );
    void OnGetEvents( const timeline::Events& events, const timeline::Error& error );
    void OnLoadedEvents( const timeline::Error& error );
    void OnSavedEvents( const std::string& content, const timeline::Error& error );

    void OnSelectedEvent( boost::shared_ptr< timeline::Event > event );
    void OnActivatedEvent( const timeline::Event& event );
    void OnContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time );
    void OnKeyUp( int key );

    void OnCreateClicked( int );

    void OnLoadOrderFileRequested( const tools::Path& filename );
    void OnSaveOrderFileRequested( const tools::Path& filename );
    void OnLoadTimelineSessionFileRequested( const tools::Path& filename );
    void OnSaveTimelineSessionFileRequested( const tools::Path& filename );

    void OnEngagedFilterToggled( bool checked );
    void OnServicesFilterChanged( const std::string& services );
    void OnKeywordFilterChanged( const std::string& keyword );
    void OnHideHierarchiesFilterChanged( const std::string& hierarchies );
    //@}

private:
    //! @name Member data
    //@{
    boost::scoped_ptr< QWidget > timelineWidget_;
    QVBoxLayout* mainLayout_;

    const GamingConfig& config_;
    kernel::Controllers& controllers_;
    Model& model_;

    boost::scoped_ptr< timeline::Server_ABC > server_;
    boost::shared_ptr< timeline::Event > selected_;
    QDateTime selectedDateTime_;
    boost::scoped_ptr< QSignalMapper > creationSignalMapper_;
    tools::Path currentFile_;
    std::string eventCreated_;
    QString lastProfile_;
    QString serverProfile_;
    bool horizontal_;
    std::string parentUuid_;
    //@}
};

#endif // __TimelineWebView_h_
