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
// Name: MIL_Config::GetConfigFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetConfigFileName() const
{
    return strConfFile_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetConfigFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetConfigFileName( const std::string& strName )
{
    strConfFile_ = strName;
}


// -----------------------------------------------------------------------------
// Name: MIL_Config::GetExerciceID
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetExerciceID() const
{
    return nExerciceID_;
}

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
// Name: MIL_Config::UseDIAArchive
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseDIAArchive() const
{
    return bUseDIAArchive_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::GetNbrPathFindThreads
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
uint MIL_Config::GetNbrPathFindThreads() const
{
    return nNbrPathFindThreads_;
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
// Name: MIL_Config::GetCheckPointFileName
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Config::GetCheckPointFileName() const
{
    return strCheckPointFileName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetExerciceID
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetExerciceID( uint nExercideID )
{
    nExerciceID_ = nExercideID;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetUseDIAArchive
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetUseDIAArchive( bool bUseDIAArchive )
{
    bUseDIAArchive_ = bUseDIAArchive;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetNbrPathFindThreads
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetNbrPathFindThreads( uint nNbr )
{
    nNbrPathFindThreads_ = nNbr;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetCheckPointFileName
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetCheckPointFileName( const std::string& strName )
{
    strCheckPointFileName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetForceODBAutomateComposition
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetForceODBAutomateComposition( bool bForce )
{
    bForceODBAutomateComposition_ = bForce;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseCheckPoint
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseCheckPoint() const
{
    return !strCheckPointFileName_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetUseDecDebug
// Created: NLD 2003-12-16
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetUseDecDebug( bool bUse )
{
    bUseDecDebug_ = bUse;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::UseDiaDebugServer
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
inline
bool MIL_Config::UseDiaDebugServer() const
{
    return bUseDiaDebugServer_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetUseDiaDebugServer
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetUseDiaDebugServer( bool bUse )
{
    bUseDiaDebugServer_ = bUse;
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
// Name: MIL_Config::SetProfilingEnabled
// Created: NLD 2004-07-15
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetProfilingEnabled( bool bEnabled )
{
    bProfilingEnabled_ = bEnabled;    
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetUseCheckPointCRC
// Created: NLD 2005-04-15
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetUseCheckPointCRC( bool bUse )
{
    bUseCheckPointCRC_ = bUse;
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
// Name: MIL_Config::SetUsePathDebug
// Created: AGE 2005-07-04
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetUsePathDebug( bool bUse )
{
    bUsePathDebug_ = bUse;
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::SetDataTestMode
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
inline
void MIL_Config::SetDataTestMode( bool bEnabled )
{
    bDataTestMode_ = bEnabled;
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
