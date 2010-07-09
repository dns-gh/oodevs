// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __XPSPlayer_h_
#define __XPSPlayer_h_

#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace kernel {
    class Controllers;
}
class Player;
class Fires;

// =============================================================================
/** @class  XPSPlayer
    @brief  XPSPlayer
*/
// Created: AGE 2006-11-13
// =============================================================================
class XPSPlayer : public QObject, public tools::Observer_ABC
                , public tools::ElementObserver_ABC< Fires >
                , public tools::ElementObserver_ABC< Simulation >
                , public tools::ElementObserver_ABC< Simulation::sStartTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             XPSPlayer( QObject* parent, kernel::Controllers& controllers );
    virtual ~XPSPlayer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const Fires& );
    virtual void NotifyUpdated( const Simulation::sStartTick& );
    virtual void NotifyUpdated( const Simulation& simu );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XPSPlayer( const XPSPlayer& );            //!< Copy constructor
    XPSPlayer& operator=( const XPSPlayer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::auto_ptr< Player > player_;
    //@}
};

#endif // __XPSPlayer_h_
