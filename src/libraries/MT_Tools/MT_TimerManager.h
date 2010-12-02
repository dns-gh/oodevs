// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_TimerManager_h_
#define __MT_TimerManager_h_

#include "MT_Timer_ABC.h"
#include <boost/noncopyable.hpp>
#include <list>

//=============================================================================
/**
    @class  MT_TimerManager
    @brief  Timer manager
  
  // $$$$ __RC__ NLD 2002-09-26: gérer le bool bThreaded

    @par    Using example
    @code
        class MyTimer : public MT_Timer_ABC
        {
            void OnTimer();
        };

        MT_TimerManager timerManager;
        MyTimer myTimer;
        timerManager.Register( myTimer );
        
        while( 1 )
            timerManager.Update();

        timerManager.Unregister( myTimer );

    @endcode
*/
// Created:  MBN 00-05-03
//=============================================================================
class MT_TimerManager : private boost::noncopyable
{

public:
    //-------------------------------------------------------------------------
    /** @name Constructor / Destructor */
    //-------------------------------------------------------------------------
    //@{
     MT_TimerManager();
    ~MT_TimerManager();
    //@}

    //-----------------------------------------------------------------------------
    /** @name Operations */
    //-----------------------------------------------------------------------------
    //@{
    void Update() const;

    void Register( MT_Timer_ABC& timer );
    void Unregister( MT_Timer_ABC& timer );
    //@}

private:
    //-----------------------------------------------------------------------------
    /** @name Types */
    //-----------------------------------------------------------------------------
    //@{
    typedef std::list< MT_Timer_ABC* >    T_Timers;
    typedef T_Timers::const_iterator    CIT_Timers;
    //@}

private:
    //-----------------------------------------------------------------------------
    /** @name Member data */
    //-----------------------------------------------------------------------------
    //@{
    T_Timers timers_;
    //@}
};

#endif // __MT_TimerManager_h_
