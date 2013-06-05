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

namespace kernel
{
    class Controllers;
}

namespace timeline
{
    struct Configuration;
    struct Error;
    struct Event;
    class Server_ABC;
}

class Config;
class Simulation;

// =============================================================================
/** @class  TimelineDockWidget
    @brief  TimelineDockWidget
*/
// Created: ABR 2013-05-14
// =============================================================================
class TimelineDockWidget : public gui::RichDockWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const Simulation& simulation );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
    //@}

private:
    //! @name Helpers
    //@{
    void Error( const QString& text );
    void CreateDummyMission(); // $$$$ ABR 2013-05-24: Test method
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEventSignal( const timeline::Event& event );
    void DeleteEventSignal( const std::string& uuid );
    //@}

private slots:
    //! @name Slots
    //@{
    void CreateEvent( const timeline::Event& event );
    void DeleteEvent( const std::string& uuid );

    void OnCreatedEvent( const timeline::Event& event, const timeline::Error& error );
    void OnDeletedEvent( const std::string& uuid, const timeline::Error& error );

    void OnSelectedEvent( boost::shared_ptr< timeline::Event > event );
    void OnActivatedEvent( const timeline::Event& event );
    void OnContextMenuEvent( boost::shared_ptr< timeline::Event > event );
    void OnKeyUp( int key );
    //@}

private:
    //! @name Member data
    //@{
    QWidget* mainWidget_;
    std::auto_ptr< timeline::Server_ABC > server_;
    std::auto_ptr< timeline::Configuration > cfg_;

    boost::shared_ptr< timeline::Event > selected_;

    std::vector< std::string > creationRequestedEvents_;
    std::vector< std::string > deletionRequestedEvents_;

    const Simulation& simulation_;
    //@}
};

#endif // __TimelineDockWidget_h_
