//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_EnemyInfo.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo::SetName
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
inline
void MOS_EnemyInfo::SetName( const char* szName )
{
    strName_ = szName;
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo::AddReferecingAutomata
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
inline
void MOS_EnemyInfo::AddReferecingAutomata( const MOS_Agent& agent )
{
    referencingAutomataSet_.insert( & agent );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo::RemoveReferecingAutomata
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
inline
void MOS_EnemyInfo::RemoveReferecingAutomata( const MOS_Agent& agent )
{
    referencingAutomataSet_.erase( & agent );
}

