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
// Name: DEC_Knowledge_PopulationFlowPart::Clean
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationFlowPart::Clean()
{
    return rRelevance_ <= 0. || shape_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPart::ChangeRelevance
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationFlowPart::ChangeRelevance( MT_Float rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return false;

    static const MT_Float rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
    {
        rRelevance_ = rNewRelevance;
        return true;
    }
    rRelevance_ = rNewRelevance;
    return false;
}
