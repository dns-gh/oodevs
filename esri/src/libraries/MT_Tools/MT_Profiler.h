//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MT_Profiler.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:53 $
// $Revision: 2 $
// $Workfile: MT_Profiler.h $
//
//*****************************************************************************

#ifndef __MT_Profiler_h_
#define __MT_Profiler_h_

#include "MT_Tools_Types.h"

//=============================================================================
// Created: NLD 2002-10-14
//=============================================================================
class MT_Profiler
{
    MT_COPYNOTALLOWED( MT_Profiler );

public:
     MT_Profiler();
    ~MT_Profiler();

    void Reset();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    static void Initialize();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool32      IsStarted() const;
    void        Start();
    double      Stop ();             // Stop the timer and return the time elapsed since the Start() method was called, in milliseconds
    double      GetLastTime();       // return last mesured time
    double      GetAverageTime();    // return average of mesured time (useful in a frame by frame systeme)
    //@}

private:
    static int64 nFrequency_;

private:
    int64   nCounterStart_;     // l'heure du start
    double  rLastTime_;         // la derniere valeur mesurée
    double  rTotalTime_;        // sum of all start/stop elapsing (in ms)
    int     nNbrCount_;         // Number of start Profiler was started (for GetAverageTime );

};

#include "MT_Profiler.inl"

#endif // __MT_Profiler_h_
