// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/Config.inl $
// $Author: Sbo $
// $Modtime: 5/07/05 11:45 $
// $Revision: 2 $
// $Workfile: Config.inl $
//
// *****************************************************************************

namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Config::GetServer
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
const std::string& Config::GetServer() const
{
    return strServer_;
}


//-----------------------------------------------------------------------------
// Name: Config::GetPort
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
uint Config::GetPort() const
{
    return nPort_;
}

//-----------------------------------------------------------------------------
// Name: Config::GetSimConfigFile
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
inline
const std::string& Config::GetSimConfigFile() const
{
    return strSimConfigFile_;
}

//-----------------------------------------------------------------------------
// Name: Config::GetExportFilename
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
inline
const std::string& Config::GetExportFilename() const
{
    return strExportFilename_;
}

//-----------------------------------------------------------------------------
// Name: Config::GetPlatformNatureFilename
// Created: SBO 2005-07-05
//-----------------------------------------------------------------------------
inline
const std::string& Config::GetPlatformNatureFilename() const
{
    return strPlatformNatureFilename_;
}

//-----------------------------------------------------------------------------
// Name: Config::MustExportCommunications
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
inline
bool Config::MustExportCommunications() const
{
    return bExportCommunications_;
}

//-----------------------------------------------------------------------------
// Name: Config::IsIncrementalExport
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
inline
bool Config::IsIncrementalExport() const
{
    return bExportIncremental_;
}

} // end namespace TICExport