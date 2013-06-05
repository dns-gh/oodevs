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
}

class Config;
class Model;
class Simulation;
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
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const Simulation& simulation, Model& model );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
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
    QTabWidget* tabWidget_;
    QString mainTitle_;
    std::vector< TimelineFilteredViewWidget* > filteredViews_;
    boost::shared_ptr< timeline::Configuration > cfg_;
    const Simulation& simulation_;
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
