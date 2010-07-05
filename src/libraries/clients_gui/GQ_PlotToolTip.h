// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GQ_PlotToolTip_h_
#define __GQ_PlotToolTip_h_

#include <qtooltip.h>

class QRect;
class QStringList;

namespace gui
{
    class GQ_Plot;

// =============================================================================
/** @class  GQ_PlotToolTip
    @brief  GQ_PlotToolTip
    @par    Using example
    @code
    GQ_PlotToolTip;
    @endcode
*/
// Created: CBX 2003-08-18
// =============================================================================
class GQ_PlotToolTip : public QToolTip
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GQ_PlotToolTip( GQ_Plot& );
    virtual ~GQ_PlotToolTip();
    //@}

protected:
    //! @name Qt overloaded methods
    //@{
    virtual void maybeTip( const QPoint& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GQ_PlotToolTip( const GQ_PlotToolTip& rhs );            //!< Copy constructor
    GQ_PlotToolTip& operator=( const GQ_PlotToolTip& rhs ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
     GQ_Plot& plot_;
    //@}
};

}

#endif // __GQ_PlotToolTip_h_
