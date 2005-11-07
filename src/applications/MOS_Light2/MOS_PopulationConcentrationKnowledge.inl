//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledge.inl $
// $Author: Ape $
// $Modtime: 28/05/04 15:43 $
// $Revision: 3 $
// $Workfile: MOS_AgentKnowledge.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetID
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentrationKnowledge::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetGtia
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const MOS_Gtia& MOS_PopulationConcentrationKnowledge::GetGtia() const
{
    assert( pGtia_ );
    return *pGtia_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetPopulation
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const MOS_PopulationKnowledge* MOS_PopulationConcentrationKnowledge::GetPopulationKnowledge() const
{
    return pPopulationKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetConcentration
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const MOS_PopulationConcentration* MOS_PopulationConcentrationKnowledge::GetConcentration() const
{
    return pConcentration_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetPosition
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationConcentrationKnowledge::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetNbrAliveHumans
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentrationKnowledge::GetNbrAliveHumans() const
{
    return nNbrAliveHumans_.value_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetNbrDeadHumans
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
uint MOS_PopulationConcentrationKnowledge::GetNbrDeadHumans() const
{
    return nNbrDeadHumans_.value_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetAttitude
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
E_PopulationAttitude MOS_PopulationConcentrationKnowledge::GetAttitude() const
{
    return eAttitude_.value_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetRelevance
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
MT_Float MOS_PopulationConcentrationKnowledge::GetRelevance() const
{
    return rRelevance_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::IsPerceived
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationConcentrationKnowledge::IsPerceived() const
{
    return bIsPerceived_.value_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::IsValidNbrAliveHumans
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationConcentrationKnowledge::IsValidNbrAliveHumans() const
{
    return nNbrAliveHumans_.bIsValid_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::IsValidNbrDeadHumans
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationConcentrationKnowledge::IsValidNbrDeadHumans() const
{
    return nNbrDeadHumans_.bIsValid_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::IsValidAttitude
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationConcentrationKnowledge::IsValidAttitude() const
{
    return eAttitude_.bIsValid_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::IsValidPerceived
// Created: SBO 2005-11-07
// -----------------------------------------------------------------------------
inline
bool MOS_PopulationConcentrationKnowledge::IsValidPerceived() const
{
    return bIsPerceived_.bIsValid_;
}
