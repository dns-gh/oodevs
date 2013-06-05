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
    class Time_ABC;
}

namespace timeline
{
    struct Configuration;
    struct Event;
}

class Config;
class EventAction;
class EventDialog;
class Model;
class TimelineFilteredViewWidget;

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
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const kernel::Time_ABC& simulation, Model& model );
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
    void CreateInstantOrder( const EventAction& );
    //@}

public slots:
    //! @name Slots
    //@{
    void AddFilteredView( QStringList filters );
    void RemoveCurrentFilteredView();
    //@}

private:
    //! @name Member data
    //@{
    EventDialog* eventDialog_;
    QTabWidget* tabWidget_;
    QString mainTitle_;
    std::vector< TimelineFilteredViewWidget* > filteredViews_;
    boost::shared_ptr< timeline::Configuration > cfg_;
    const kernel::Time_ABC& simulation_;
    Model& model_;
    bool isConnected_;
    //@}

private:
    //! @name Static member data
    //@{
    static int maxTabNumber_;
    //@}
};

#endif // __TimelineDockWidget_h_
