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
class AfterActionRequest;

// =============================================================================
/** @class  AfterActionPlot
    @brief  AfterActionPlot
*/
// Created: AGE 2007-09-26
// =============================================================================
class AfterActionPlot : public GQ_Plot
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionPlot( QWidget* parent, QDockWindow* dock );
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
    //@}

    //! @name Types
    //@{
    typedef std::vector< GQ_PlotData* > T_Datas;
    //@}

private:
    //! @name Member data
    //@{
    QDockWindow* dock_;
    T_Datas datas_;
    double min_;
    double max_;
    //@}
};

#endif // __AfterActionPlot_h_
