// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTROLLER_H__
#define CONTROLLER_H__

#include <timeline/api.h>
#include <memory>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace Ui
{
    class Main;
}

class Controller : public QObject
{
    Q_OBJECT
public:
             Controller( const timeline::Configuration& cfg );
    virtual ~Controller();

    int  Execute( const std::string& command, const std::vector< std::string >& args );
    void Show();

public slots:
    void OnReload();
    void OnLoad();
    void OnReady();
    void OnCenter();
    void OnSwitchLayout();
    void OnCreateEvent();
    void OnCreatedEvents( const timeline::Events& events, const timeline::Error& error );
    void OnSelectedEvent( boost::shared_ptr< timeline::Event > event );
    void OnDeletedEvents( const std::vector< std::string >& uuids, const timeline::Error& error );
    void OnDeleteEvent();
    void OnTestCrud();
    void OnActivatedEvent( const timeline::Event& event );
    void OnContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time );
    void OnKeyDown( int key );
    void OnKeyPress( int key );
    void OnKeyUp( int key );
    void OnLoadEvents();
    void OnLoadActionTriggered();
    void OnLoadedEvents( const timeline::Error& error );
    void OnSavedEvents( const std::string& events, const timeline::Error& error );
    void OnSelect();
    void OnReadEvents();

public:
    void WaitReady() const;
    int Create( const std::vector< std::string >& args );
    int Read( const std::vector< std::string >& args );
    int Update( const std::vector< std::string >& args );
    int Delete( const std::vector< std::string >& args );
    int SaveLoad( const std::vector< std::string >& args );

private:
    QMainWindow main_;
    std::auto_ptr< Ui::Main > ui_;
    std::auto_ptr< timeline::Server_ABC > ctx_;
    std::string uuid_;
    QLineEdit url_;
    QDialog loadEventsDialog_;
    QTextEdit loadEventsText_;
    std::string command_;
    bool usePopups_;
};

class OnSignal_ABC : public QObject
{
    Q_OBJECT
public:
             OnSignal_ABC() {}
    virtual ~OnSignal_ABC() {}
public slots:
    virtual void OnCreatedEvents( const timeline::Events& /*events*/, const timeline::Error& /*error*/ ) {}
    virtual void OnReadEvents   ( const timeline::Events& /*events*/, const timeline::Error& /*error*/ ) {}
    virtual void OnReadEvent    ( const timeline::Event&  /*event*/,  const timeline::Error& /*error*/ ) {}
    virtual void OnUpdatedEvent ( const timeline::Event&  /*event*/,  const timeline::Error& /*error*/ ) {}
    virtual void OnDeletedEvents( const std::vector< std::string >& /*uuids*/, const timeline::Error& /*error*/ ) {}
    virtual void OnLoadedEvents ( const timeline::Error& /*error*/ ) {}
    virtual void OnSavedEvents  ( const std::string& /*events*/, const timeline::Error& /*error*/ ) {}
};

#endif//CONTROLLER_H__
