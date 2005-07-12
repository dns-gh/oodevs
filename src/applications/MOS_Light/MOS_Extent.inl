//*****************************************************************************
// Name   : MOS_Extent.inl
// Created: FBD 02-04-19
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Extent::GetMeterPerPixel
// Created: FBD 02-11-14
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Extent::GetMeterPerPixel() const
{
    return rMeterPerPixel_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Extent::GetRectDMS
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
const MT_Rect& MOS_Extent::GetRectDMS() const
{
    return rectDMS_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Extent::GetRect
// Created: FBD 02-11-07
//-----------------------------------------------------------------------------
inline
const MT_Rect& MOS_Extent::GetRect() const
{
    return rect_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetTopDMSAsString() const
{
    return strDMSTop_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetBottomDMSAsString() const
{
    return strDMSBottom_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetLeftDMSAsString() const
{
    return strDMSLeft_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetRightDMSAsString() const
{
    return strDMSRight_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetWidthPixelAsString() const
{
    return strWidthPixel_;
}


//-----------------------------------------------------------------------------
// Name: GetHeightPixelAsString
// Created: JVT 02-11-05
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Extent::GetHeightPixelAsString() const
{
    return strHeightPixel_;
}
