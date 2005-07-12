//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_Workspace.inl $
//   $Author: Aml $
//  $Modtime: 25/07/03 12:06 $
// $Revision: 9 $
// $Workfile: LMP_Workspace.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Message To Send (tous envoyé au launcher)
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name   : LMP_Workspace::SendMsg_RequestListMission
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
INLINE
bool32 LMP_Workspace::SendMsg_RequestListMission()
{
    return pLau_->SendMsg_MosLau_RequestListMission();
}



//-----------------------------------------------------------------------------
// Name: LMP_Workspace::SendMsg_RequestListSim
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
bool32 LMP_Workspace::SendMsg_RequestListSim()
{
    return pLau_->SendMsg_MosLau_RequestListSim();
}



//-----------------------------------------------------------------------------
// Name: LMP_Workspace::SendMsg_StopSim
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
INLINE
bool32 LMP_Workspace::SendMsg_StopSim( const uint32 nExerciceID )
{
    return pLau_->SendMsg_MosLau_StopSim( nExerciceID );
}


//-----------------------------------------------------------------------------
// Name: LMP_Workspace::SendMsg_RequestListCheckpoint
// Created: AML 03-07-25
//-----------------------------------------------------------------------------
INLINE
bool32 LMP_Workspace::SendMsg_RequestListCheckpoint( const uint32 nExerciceID )
{
    return pLau_->SendMsg_MosLau_RequestListCheckpoint( nExerciceID );
}

