//*****************************************************************************
//
// $Created: NLD 2003-01-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Net_Def.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: Net_Def.h $
//
//*****************************************************************************

#ifndef __Net_Def_h_
#define __Net_Def_h_

enum MIL_Connector
{
    eConnector_SIM_MOS = (DIN::DIN_Connector_ABC::DIN_ConnectionID)-1,
    eConnector_SIM_LAU = 1
};

//=============================================================================
// OPERATORS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: operator <<
// Created: NLD 2002-12-13
//-----------------------------------------------------------------------------
inline
NEK::NEK_Output& operator << ( NEK::NEK_Output& msg, const MT_Vector2D& vPos )
{
    return msg << vPos.rX_ << vPos.rY_;
}

//-----------------------------------------------------------------------------
// Name: operator >>
// Created: NLD 2002-12-13
//-----------------------------------------------------------------------------
inline
NEK::NEK_Input&  operator >> ( NEK::NEK_Input& msg, MT_Vector2D& vPos )
{
    return msg >> vPos.rX_ >> vPos.rY_;
}

#endif // __Net_Def_h_
