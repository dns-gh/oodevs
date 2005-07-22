//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/SSC/src/MT_Profiler.h $
// $Author: Agn $
// $Modtime: 13/10/03 10:47 $
// $Revision: 1 $
// $Workfile: MT_Profiler.h $
//
//*****************************************************************************

#ifndef __MT_Profiler_h_
#define __MT_Profiler_h_

#include "MT/MT_Tools/MT_Types.h"

//=============================================================================
/**
*   @class  MT_Profiler
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
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
    bool      IsStarted() const;
    void        Start();
    double      Stop ();             // Stop the timer and return the time elapsed since the Start() method was called, in milliseconds
    double      GetLastTime();       // return last mesured time
    int         GetAverageTime();    // return average of mesured time (useful in a frame by frame systeme)
    //@}

private:
    static int64 nFrequency_;

private:
    int64   nCounterStart_;     // l'heure du start
    double  rLastTime_;         // la derniere valeur mesurée
    int     nTotalTime_;        // sum of all start/stop elapsing (in ms)
    int     nNbrCount_;         // Number of start Profiler was started (for GetAverageTime );

};


//-----------------------------------------------------------------------------
// Name: MT_Profiler::IsStarted
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
bool MT_Profiler::IsStarted() const
{
    return nCounterStart_ != 0;
}


//-----------------------------------------------------------------------------
// Name: MT_Profiler::GetLastTime
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
double MT_Profiler::GetLastTime()
{
    return rLastTime_;
}


//-----------------------------------------------------------------------------
// Name: MT_Profiler::GetAverageTime
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
int MT_Profiler::GetAverageTime()
{
    if( nNbrCount_ == 0 )
       return 0;

    return nTotalTime_ / nNbrCount_;
}


#endif // __MT_Profiler_h_
