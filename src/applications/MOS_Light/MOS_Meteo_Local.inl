//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Local.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::GetId
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_Local::GetId() const
{
    return nId_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::GetUpperLeftCorner
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_Meteo_Local::GetUpperLeftCorner()
{
    return vUpperLeftCorner_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local::GetLowerRightCorner
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
MT_Vector2D& MOS_Meteo_Local::GetLowerRightCorner()
{
    return vLowerRightCorner_;
}

