//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MT_Profiler.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 14:53 $
// $Revision: 2 $
// $Workfile: MT_Profiler.inl $
//
//*****************************************************************************


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
double MT_Profiler::GetLastTime() const
{
    return rLastTime_;
}

//-----------------------------------------------------------------------------
// Name: MT_Profiler::GetAverageTime
// Created: AML 03-05-21
//-----------------------------------------------------------------------------
inline
double MT_Profiler::GetAverageTime() const
{
    return nNbrCount_ ? rTotalTime_ / static_cast< double >( nNbrCount_ ) : 0;
}
