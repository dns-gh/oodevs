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

//=============================================================================
// Created: NLD 2002-10-14
//=============================================================================
class MT_Profiler
{

public:
    MT_Profiler();

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
    bool        IsStarted() const;
    void        Start();
    double      Stop ();                // Stop the timer and return the time elapsed since the Start() method was called, in milliseconds
    double      GetLastTime() const;    // Return last mesured time
    double      GetAverageTime() const; // Return average of mesured time (useful in a frame by frame system)
    double      GetTotalTime() const;
    int         GetCount() const;
    //@}

private:
    long long int nCounterStart_; // Start time
    double rLastTime_;            // Last measured value (in ms)
    double rTotalTime_;           // Sum of all start/stop elapsing (in ms)
    int nNbrCount_;               // Number of start Profiler was started (for GetAverageTime);

};

#include "MT_Profiler.inl"

#endif // __MT_Profiler_h_
