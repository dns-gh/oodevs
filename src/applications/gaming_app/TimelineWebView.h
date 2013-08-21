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
#include "timeline/api.h"

namespace kernel
{
    class ContextMenu;
    class ActionController;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineWebView( QWidget* parent, const tools::ExerciseConfig& config, kernel::ActionController& actionController,
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
    void CreateDummyEvent( E_EventTypes type ); // $$$$ ABR 2013-05-24: Test method
    Event& GetOrCreateEvent( const timeline::Event& event );

    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );

    void ReadActions( xml::xistream& xis );
    void ReadAction( xml::xistream& xis );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEventSignal( const timeline::Event& event );
    void EditEventSignal( const timeline::Event& event );
    void DeleteEventSignal( const std::string& uuid );

    void StartCreation( E_EventTypes type, const QDateTime& dateTime );
    //@}

private slots:
    //! @name Slots
    //@{
    void CreateEvent( const timeline::Event& event );
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
    void OnCreateDummyClicked( int );

    void OnLoadOrderFileRequested( const tools::Path& filename );
    void OnSaveOrderFileRequested( const tools::Path& filename );
    void OnLoadTimelineSessionFileRequested( const tools::Path& filename );
    void OnSaveTimelineSessionFileRequested( const tools::Path& filename );
    //@}

private:
    //! @name Member data
    //@{
    QWidget* timelineWidget_;
    QVBoxLayout* mainLayout_;

    const tools::ExerciseConfig& config_;
    kernel::ActionController& actionController_;
    Model& model_;

    std::auto_ptr< timeline::Server_ABC > server_;
    std::auto_ptr< timeline::Configuration > cfg_;
    boost::shared_ptr< timeline::Event > selected_;
    QDateTime selectedDateTime_;
    QSignalMapper* creationSignalMapper_;
    QSignalMapper* dummySignalMapper_;
    tools::Path currentFile_;

    std::vector< std::string > creationRequestedEvents_;
    std::vector< std::string > editionRequestedEvents_;
    std::vector< std::string > deletionRequestedEvents_;
    //@}
};

#endif // __TimelineWebView_h_
