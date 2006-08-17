//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_SiteDecontamination.inl $
// $Author: Nld $
// $Modtime: 23/03/05 11:07 $
// $Revision: 2 $
// $Workfile: MIL_SiteDecontamination.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_SiteDecontamination::Create()
{
    return *new MIL_SiteDecontamination();
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination::QueueForDecontamination
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
void MIL_SiteDecontamination::QueueForDecontamination( MIL_Agent_ABC& agent )
{
    decontaminationQueue_.insert( &agent );
}

