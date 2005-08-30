// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-24 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: Config::GetServer
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
const std::string& Config::GetServer() const
{
    return strServer_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetConfigFile
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
const std::string& Config::GetConfigFile() const
{
    return strConfigFile_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetRandomSeedFile
// Created: SBO 2005-08-29
// -----------------------------------------------------------------------------
inline
const std::string& Config::GetRandomSeedFile() const
{
    return strRandomSeedFile_;
}

// -----------------------------------------------------------------------------
// Name: Config::MustRecover
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
bool Config::MustRecover() const
{
    return bRecover_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetRecoveryTick
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
uint Config::GetRecoveryTick() const
{
    return nRecoveryTick_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetRecoveryFile
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
inline
const std::string& Config::GetRecoveryFile() const
{
    return strRecoveryFile_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetPort
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
uint Config::GetPort() const
{
    return nPort_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetTimeFactor
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
uint Config::GetTimeFactor() const
{
    return nTimeFactor_;
}

// -----------------------------------------------------------------------------
// Name: Config::MustTestAutomat
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
bool Config::MustTestAutomat() const
{
    return bTestAutomat_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetPeriod
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
uint Config::GetPeriod() const
{
    return nPeriod_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetIterationNumber
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
uint Config::GetIterationNumber() const
{
    return nItNumber_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetIterationInterval
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
inline
uint Config::GetIterationInterval() const
{
    return nItInterval_;
}

// -----------------------------------------------------------------------------
// Name: Config::GetLogFile
// Created: SBO 2005-08-25
// -----------------------------------------------------------------------------
inline
const std::string& Config::GetLogFile() const
{
    return strLogFile_;
}

} // end namespace TEST