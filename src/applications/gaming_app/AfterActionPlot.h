// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionPlot_h_
#define __AfterActionPlot_h_

#include "GQ_Plot.h"
#include "clients_kernel/ElementObserver_ABC.h"
namespace kernel
{
    class Controllers;
}

class AfterActionRequest;
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  AfterActionPlot
    @brief  AfterActionPlot
*/
// Created: AGE 2007-09-26
// =============================================================================
class AfterActionPlot : public GQ_Plot
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< Simulation >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionPlot( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, QDockWindow* dock );
    virtual ~AfterActionPlot();
    //@}

    //! @name Operations
    //@{
    void Add( const AfterActionRequest& request );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionPlot( const AfterActionPlot& );            //!< Copy constructor
    AfterActionPlot& operator=( const AfterActionPlot& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void hideEvent( QHideEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* e );
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

    //! @name Types
    //@{
    typedef std::vector< GQ_PlotData* > T_Datas;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    QDockWindow* dock_;
    T_Datas datas_;
    GQ_PlotData* tickData_;
    double min_;
    double max_;
    //@}
};

#endif // __AfterActionPlot_h_
