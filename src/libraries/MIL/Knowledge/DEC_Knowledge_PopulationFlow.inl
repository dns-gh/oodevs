// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlow.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlow.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlow::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationFlow::Clean()
{
    bHumansUpdated_    = false;
    bAttitudeUpdated_  = false;  
    bRealFlowUpdated_  = false;
    bRelevanceUpdated_ = false;
    bShapeUpdated_     = false;
    bSpeedUpdated_     = false;
    bDirectionUpdated_ = false;
    return false;
}