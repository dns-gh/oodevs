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
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

class Config;
class Model;
class TimelineWebView;

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
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, Model& model );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    void EditEvent( const timeline::Event& );
    void DeleteEvent( const std::string& );

    void StartCreation( E_EventTypes, const QDateTime& );
    //@}

public slots:
    //! @name Slots
    //@{
    void AddView();
    void RemoveCurrentView();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::auto_ptr< timeline::Configuration > cfg_;

    QTabWidget* tabWidget_;
    std::vector< std::pair< int, QWidget* > > toolbars_;
    TimelineWebView* webView_;
    //@}

private:
    //! @name Static member data
    //@{
    static int maxTabNumber_;
    //@}
};

#endif // __TimelineDockWidget_h_
