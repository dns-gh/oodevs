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
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Filter_ABC;
}

namespace timeline
{
    struct Configuration;
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

class Config;
class Model;
class TimelineWebView;
class TimelineToolBar;

// =============================================================================
/** @class  TimelineDockWidget
    @brief  TimelineDockWidget
*/
// Created: ABR 2013-05-14
// =============================================================================
class TimelineDockWidget : public gui::RichDockWidget
                         , public tools::ElementObserver_ABC< kernel::Filter_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, Model& model );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();

    virtual void NotifyCreated( const kernel::Filter_ABC& filter );
    virtual void NotifyUpdated( const kernel::Filter_ABC& filter );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    void SelectEvent( const std::string& );
    void EditEvent( const timeline::Event& );
    void DeleteEvent( const std::string& );

    void StartCreation( E_EventTypes, const QDateTime&, bool );
    //@}

public slots:
    //! @name Slots
    //@{
    void AddView( bool main = false );
    void RemoveCurrentView();
    void OnCurrentChanged( int index );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::auto_ptr< timeline::Configuration > cfg_;
    QTabWidget* tabWidget_;
    TimelineWebView* webView_;
    //@}

private:
    //! @name Static member data
    //@{
    static int maxTabNumber_;
    //@}
};

#endif // __TimelineDockWidget_h_
