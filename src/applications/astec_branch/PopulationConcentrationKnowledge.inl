//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledge.inl $
// $Author: Ape $
// $Modtime: 28/05/04 15:43 $
// $Revision: 3 $
// $Workfile: AgentKnowledge.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetID
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
inline
uint PopulationConcentrationKnowledge::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetKnowledgeGroup
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const KnowledgeGroup& PopulationConcentrationKnowledge::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetPopulation
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const PopulationKnowledge* PopulationConcentrationKnowledge::GetPopulationKnowledge() const
{
    return pPopulationKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetConcentration
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const PopulationConcentration* PopulationConcentrationKnowledge::GetConcentration() const
{
    return pConcentration_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetPosition
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PopulationConcentrationKnowledge::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetNbrAliveHumans
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
uint PopulationConcentrationKnowledge::GetNbrAliveHumans() const
{
    return nNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetNbrDeadHumans
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
uint PopulationConcentrationKnowledge::GetNbrDeadHumans() const
{
    return nNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetAttitude
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
E_PopulationAttitude PopulationConcentrationKnowledge::GetAttitude() const
{
    return eAttitude_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetRelevance
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
MT_Float PopulationConcentrationKnowledge::GetRelevance() const
{
    return rRelevance_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::IsPerceived
// Created: SBO 2005-10-20
// -----------------------------------------------------------------------------
inline
bool PopulationConcentrationKnowledge::IsPerceived() const
{
    return bIsPerceived_;
}
