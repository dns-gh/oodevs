// *****************************************************************************
//
// $Created: NLD 2003-11-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Config.inl $
// $Author: Nld $
// $Modtime: 15/04/05 17:17 $
// $Revision: 5 $
// $Workfile: MIL_Config.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseDecDebug
// Created: NLD 2003-12-16
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseDecDebug() const
{
    return bUseDecDebug_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::CheckAutomateComposition
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::CheckAutomateComposition() const
{
    return bCheckAutomateComposition_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsDecisionalProfilingEnabled
// Created: SLI 2012-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsDecisionalProfilingEnabled() const
{
    return bDecisionalProfilingEnabled_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsDecisionalLoggerEnabled
// Created: SLI 2013-02-11
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsDecisionalLoggerEnabled() const
{
    return bDecisionalLoggerEnabled_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsHookProfilingEnabled
// Created: SLI 2012-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsHookProfilingEnabled() const
{
    return bHookProfilingEnabled_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsCommandProfilingEnabled
// Created: SLI 2012-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsCommandProfilingEnabled() const
{
    return bCommandProfilingEnabled_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseCheckPointCRC
// Created: NLD 2005-04-15
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseCheckPointCRC() const
{
    return bUseCheckPointCRC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UsePathDebug
// Created: AGE 2005-07-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UsePathDebug() const
{
    return bUsePathDebug_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsSaveCheckpointTestMode
// Created: JSR 2010-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsSaveCheckpointTestMode() const
{
    return bSaveCheckpointTestMode_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsDispatcherEmbedded
// Created: NLD 2006-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsDispatcherEmbedded() const
{
    return bEmbeddedDispatcher_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseNetworkLogger
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseNetworkLogger() const
{
    return bUseNetworkLogger_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetNetworkLoggerPort
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
unsigned short MIL_Config::GetNetworkLoggerPort() const
{
    return networkLoggerPort_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckPointsFrequency
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetCheckPointsFrequency() const
{
    return checkPointsFrequency_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckPointsKept
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetCheckPointsKept() const
{
    return checkPointsKept_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTimeStep
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetTimeStep() const
{
    return timeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTimeFactor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetTimeFactor() const
{
    return timeFactor_;
}

inline
unsigned int MIL_Config::GetGarbageCollectorPause() const
{
    return setpause_;
}

inline
unsigned int MIL_Config::GetGarbageCollectorStepMul() const
{
    return setstepmul_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetEndTick
// Created: JSR 2010-03-02
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetEndTick() const
{
    return endTick_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTickLatency
// Created: MCO 2012-11-08
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetTickLatency() const
{
    return tickLatency_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPausedAtStartup
// Created: JSR 2010-07-20
// -----------------------------------------------------------------------------
inline
bool MIL_Config::GetPausedAtStartup() const
{
    return bPausedAtStartup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetNetworkAddress
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetNetworkAddress() const
{
    return networkAddress_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetNetworkTimeout
// Created: JSR 2011-10-19
// -----------------------------------------------------------------------------
inline
unsigned long MIL_Config::GetNetworkTimeout() const
{
    return networkTimeOut_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPathFinderThreads
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
inline
unsigned int MIL_Config::GetPathFinderThreads() const
{
    return pathFinderThreads_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckpointTestName
// Created: JSR 2010-03-11
// -----------------------------------------------------------------------------
inline
const tools::Path& MIL_Config::GetCheckpointNameTestMode() const
{
    return strCheckPointNameTestMode_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetRandomSeed
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
int MIL_Config::GetRandomSeed() const
{
    return randomSeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetRandomGaussian
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
const bool* MIL_Config::GetRandomGaussian() const
{
    return randomGaussian_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetRandomDeviation
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
const double* MIL_Config::GetRandomDeviation() const
{
    return randomDeviation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetRandomMean
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
inline
const double* MIL_Config::GetRandomMean() const
{
    return randomMean_;
}
