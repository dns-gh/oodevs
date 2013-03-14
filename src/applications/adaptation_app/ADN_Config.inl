//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_DialogLog.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_DialogLog.cpp $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_Config::SetSimPath
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
inline
void ADN_Config::SetSimPath( const tools::Path& strPath )
{
    strSimPath_ = strPath;
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::SetSimArguments
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
inline
void ADN_Config::SetSimArguments( const std::wstring& strArgs )
{
    strSimArguments_ = strArgs;
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::GetSimPath
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
inline
const tools::Path& ADN_Config::GetSimPath() const
{
    return strSimPath_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Config::GetSimArguments
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
inline
const std::wstring& ADN_Config::GetSimArguments() const
{
    return strSimArguments_;
}
