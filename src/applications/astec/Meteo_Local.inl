//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Local.inl $
// $Author: Ape $
// $Modtime: 20/07/04 17:42 $
// $Revision: 2 $
// $Workfile: Meteo_Local.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: Meteo_Local::GetId
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
int Meteo_Local::GetId() const
{
    return nId_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local::GetUpperLeftCorner
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
MT_Vector2D& Meteo_Local::GetUpperLeftCorner()
{
    return vUpperLeftCorner_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_Local::GetLowerRightCorner
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
MT_Vector2D& Meteo_Local::GetLowerRightCorner()
{
    return vLowerRightCorner_;
}

