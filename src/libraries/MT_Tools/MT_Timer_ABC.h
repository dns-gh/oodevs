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

#include <boost/noncopyable.hpp>

//=============================================================================
/** @class  MT_Timer_ABC
    @brief  Timer base class
*/
// Created:  MBN 00-05-03
//=============================================================================
class MT_Timer_ABC : private boost::noncopyable
{

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

    void Wait();
    void Continue();

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
    bool bWaiting_;
    //@}
};

#endif // __MT_Timer_ABC_h_
