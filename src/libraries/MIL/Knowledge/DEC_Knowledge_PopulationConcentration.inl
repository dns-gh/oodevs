// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentration.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationConcentration.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_PopulationConcentration::Clean()
{
    bHumansUpdated_            = false;
    bAttitudeUpdated_          = false;  
    bRealConcentrationUpdated_ = false;
    bRelevanceUpdated_         = false;
    return rRelevance_ <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::ChangeRelevance
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_PopulationConcentration::ChangeRelevance( MT_Float rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return;

    static const MT_Float rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
        bRelevanceUpdated_ = true;

    rRelevance_ = rNewRelevance;
}