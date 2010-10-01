// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_Timer_ABC_h_
#define __MT_Timer_ABC_h_

#include "MT_Tools_Types.h"

#define MT_TIMER_INFINITE (uint)-1

//=============================================================================
/** @class  MT_Timer_ABC
    @brief  Timer base class
*/
// Created:  MBN 00-05-03 
//=============================================================================
class MT_Timer_ABC
{
    MT_COPYNOTALLOWED( MT_Timer_ABC );

public:
    //-----------------------------------------------------------------------------
    /** @name Constructor / Destructor */
    //-----------------------------------------------------------------------------
    //@{
             MT_Timer_ABC();
    virtual ~MT_Timer_ABC();
    //@}

    //-----------------------------------------------------------------------------
    /** @name Events */
    //-----------------------------------------------------------------------------
    //@{
    virtual void OnTimer() = 0; //!< Called at the frequency specified in StartTimer()
    //@}

    //-----------------------------------------------------------------------------
    /** @name Operations */
    //-----------------------------------------------------------------------------
    //@{
    void Start( long long int tPeriod );
    void Stop ();

    void Pause ();
    void Resume();

    void Process( long long int currentTime );
    //@}

private:
    //-----------------------------------------------------------------------------
    /** @name Member data */
    //-----------------------------------------------------------------------------
    //@{
    long long int tPeriod_;
    long long int tLastTimeLaunched_;
    bool bStarted_;
    bool bPaused_;
    //@}
};

#endif // __MT_Timer_ABC_h_
