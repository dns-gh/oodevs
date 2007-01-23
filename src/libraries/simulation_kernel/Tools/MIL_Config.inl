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
// Name: MIL_Config::UseOnlyDIAArchive
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseOnlyDIAArchive() const
{
    return bUseOnlyDIAArchive_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::ForceODBAutomateComposition
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::ForceODBAutomateComposition() const
{
    return bForceODBAutomateComposition_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckPointName
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetCheckPointName() const
{
    return strCheckPointName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseCheckPoint
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseCheckPoint() const
{
    return bCheckPoint_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseDiaDebugger
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseDiaDebugger() const
{
    return bUseDiaDebugger_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetDiaDebuggerPort
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint16 MIL_Config::GetDiaDebuggerPort() const
{
    return diaDebuggerPort_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsProfilingEnabled
// Created: NLD 2004-07-15
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsProfilingEnabled() const
{
    return bProfilingEnabled_;
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
// Name: MIL_Config::IsDataTestMode
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsDataTestMode() const
{
    return bDataTestMode_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsTestMode
// Created: NLD 2007-01-15
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsTestMode() const
{
    return bTestMode_;
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
uint16 MIL_Config::GetNetworkLoggerPort() const
{
    return networkLoggerPort_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckPointsFrequency
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetCheckPointsFrequency() const
{
    return checkPointsFrequency_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetCheckPointsKept
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetCheckPointsKept() const
{
    return checkPointsKept_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTimeStep
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetTimeStep() const
{
    return timeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetTimeFactor
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetTimeFactor() const
{
    return timeStep_; 
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetNetworkPort
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
uint16 MIL_Config::GetNetworkPort() const
{
    return networkPort_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsThreadedNetwork
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsThreadedNetwork() const
{
    return bNetworkThreaded_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetPathFinderThreads
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetPathFinderThreads() const
{
    return pathFinderThreads_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::IsHLAEnabled
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
inline
bool MIL_Config::IsHLAEnabled() const
{
    return bHLAEnabled_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetHLAFederation
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetHLAFederation() const
{
    return hlaFederation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetHLAFederate
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetHLAFederate() const
{
    return hlaFederate_;
}
