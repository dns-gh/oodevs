//*****************************************************************************
//
// $Created: JVT 03-02-04 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_ScipioException.inl $
// $Author: Nld $
// $Modtime: 1/09/04 16:46 $
// $Revision: 3 $
// $Workfile: MT_ScipioException.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
const std::string& MT_ScipioException::GetContext() const
{
    return strContext_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
const std::string& MT_ScipioException::GetFile() const
{
    return strFile_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
const std::string& MT_ScipioException::GetMsg() const
{
    return strMessage_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
const std::string& MT_ScipioException::GetDescription() const
{
    return strDescription_;
}


//-----------------------------------------------------------------------------
// Name: MT_ScipioException::GetLine
// Created: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
uint MT_ScipioException::GetLine() const
{
    return nLine_;
}
