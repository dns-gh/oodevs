// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ContourLinesObserver_h_
#define __ContourLinesObserver_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  __ContourLinesObserver_h_
    @brief  __ContourLinesObserver_h_
*/
// Created: JSR 2012-02-08
// =============================================================================
class ContourLinesObserver : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ContourLinesObserver() : percentage_( 0 ) {}
    virtual ~ContourLinesObserver() {}
    //@}

    //! @name Operations
    //@{
    void SetPercentage( short percentage ) { percentage_ = percentage; }
    short GetPercentage() const { return percentage_; }
    //@}

private:
    //! @name Member data
    //@{
    short percentage_;
    //@}
};

}

#endif // __ContourLinesObserver_h_
