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

#include "ENT/ENT_Enums_Gen.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/scoped_ptr.hpp>

namespace kernel
{
    class Controllers;
    class ContextMenu;
    class Profile_ABC;
    class Filter_ABC;
}

namespace timeline
{
    struct Configuration;
    struct Error;
    struct Event;
    typedef std::vector< Event > Events;
    class Server_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class Config;
class Event;
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
                      , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineWebView( QWidget* parent, const tools::ExerciseConfig& config, kernel::Controllers& controllers,
                              Model& model, timeline::Configuration& cfg );
    virtual ~TimelineWebView();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
    //@}

private:
    //! @name Helpers
    //@{
    Event& GetOrCreateEvent( const timeline::Event& event );

    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );
    virtual void NotifyCreated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyCreated( const kernel::Filter_ABC& filter );
    virtual void NotifyUpdated( const kernel::Filter_ABC& filter );

    void SetProfile( const QString& profile );

    void ReadActions( xml::xistream& xis );
    void ReadAction( xml::xistream& xis );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEventSignal( const timeline::Event& event );
    void SelectEventSignal( const std::string& uuid );
    void EditEventSignal( const timeline::Event& event );
    void DeleteEventSignal( const std::string& uuid );

    void StartCreation( E_EventTypes type, const QDateTime& dateTime );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCenterView();
    void OnSetLayoutOrientation( bool horizontal );

    void CreateEvent( const timeline::Event& event );
    void SelectEvent( const std::string& uuid );
    void EditEvent( const timeline::Event& event );
    void DeleteEvent( const std::string& uuid );

    void OnCreatedEvent( const timeline::Event& event, const timeline::Error& error );
    void OnEditedEvent( const timeline::Event& event, const timeline::Error& error );
    void OnDeletedEvent( const std::string& uuid, const timeline::Error& error );
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
    //@}

private:
    //! @name Member data
    //@{
    boost::scoped_ptr< QWidget > timelineWidget_;
    QVBoxLayout* mainLayout_;

    const tools::ExerciseConfig& config_;
    kernel::Controllers& controllers_;
    Model& model_;

    boost::scoped_ptr< timeline::Server_ABC > server_;
    boost::scoped_ptr< timeline::Configuration > cfg_;
    boost::shared_ptr< timeline::Event > selected_;
    QDateTime selectedDateTime_;
    boost::scoped_ptr< QSignalMapper > creationSignalMapper_;
    tools::Path currentFile_;
    std::string eventCreated_;
    QString lastProfile_;
    QString serverProfile_;
    std::string entityFilter_;
    //@}
};

#endif // __TimelineWebView_h_
