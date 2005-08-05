//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_ABC.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 9 $
// $Workfile: MOS_Mission_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::GetAgent
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
MOS_Agent* MOS_Mission_ABC::GetAgent() const
{
    return pAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Mission_ABC::GetTypeMission
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
inline
E_MissionID MOS_Mission_ABC::GetTypeMission() const
{
    return nTypeMission_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
inline
std::string MOS_Mission_ABC::GetName() const
{
    return std::string();
}
