// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndicatorPlot_h_
#define __IndicatorPlot_h_

#include "clients_gui/GQ_Plot.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

class IndicatorExportDialog;
class IndicatorRequest;
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  IndicatorPlot
    @brief  IndicatorPlot
*/
// Created: AGE 2007-09-26
// =============================================================================
class IndicatorPlot : public gui::GQ_Plot
                    , public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< Simulation >
                    , public tools::ElementObserver_ABC< IndicatorRequest >
                    , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorPlot( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, QDockWindow* dock,
                            IndicatorExportDialog& exportDialog, bool interactive, const IndicatorRequest& request, double currentTick,
                            QVBox* plotNames );
    virtual ~IndicatorPlot();
    //@}

    //! @name Operations
    //@{
    void Add( const IndicatorRequest& request );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnExportData();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void hideEvent( QHideEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* e );
    virtual void dragEnterEvent( QDragEnterEvent* e );
    virtual void dropEvent( QDropEvent* e );
    virtual void contextMenuEvent( QContextMenuEvent* e );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const IndicatorRequest& request );
    virtual void NotifyDeleted( const IndicatorRequest& request );
    void UpdatePlot( gui::GQ_PlotData* plot, const IndicatorRequest& request, unsigned int from );
    void SetTickData( double currentTick );
    //@}

    //! @name Types
    //@{
    typedef std::vector< gui::GQ_PlotData* > T_Datas;
    typedef std::map< const IndicatorRequest*, gui::GQ_PlotData* > T_PlottedRequests;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const bool interactive_;
    QDockWindow* dock_;
    IndicatorExportDialog& exportDialog_;
    T_Datas datas_;
    gui::GQ_PlotData* tickData_;
    T_PlottedRequests plots_;
    double min_;
    double max_;
    QLabel* plotNames_;
    //@}
};

#endif // __IndicatorPlot_h_
