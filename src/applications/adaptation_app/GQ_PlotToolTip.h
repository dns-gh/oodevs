// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-08-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotToolTip.h $
// $Author: Ape $
// $Modtime: 17/12/04 11:53 $
// $Revision: 1 $
// $Workfile: GQ_PlotToolTip.h $
//
// *****************************************************************************

#ifndef __GQ_PlotToolTip_h_
#define __GQ_PlotToolTip_h_

#include <boost/noncopyable.hpp>

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
class GQ_PlotToolTip : private boost::noncopyable
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
    //! @name Member data
    //@{
     GQ_Plot& plot_;
    //@}
};

#endif // __GQ_PlotToolTip_h_
