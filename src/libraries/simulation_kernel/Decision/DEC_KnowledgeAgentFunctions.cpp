// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeAgentFunctions.cpp $
// $Author: Jvt $
// $Modtime: 23/05/05 16:32 $
// $Revision: 8 $
// $Workfile: DEC_KnowledgeAgentFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeAgentFunctions.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"
#include "Knowledge/QueryValidity.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetNatureAtlas
// Created: NLD 2004-03-31
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::GetNatureAtlas( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetNatureAtlas().GetID() );
    return static_cast< int >( PHY_NatureAtlas::notDefined_.GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosity
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetDangerosity( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< float >( pKnowledge->GetDangerosity( callerAgent ) + 1.f );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetPotentialAttrition
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetPotentialAttrition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< MT_Vector2D > position )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< float >( callerAgent.GetDangerosity( pKnowledge, position ) );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInDetectionCone
// Created: JVT 2005-08-23
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsInDetectionCone( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const MT_Vector2D* direction, double angle )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    assert( direction );
    const MT_Vector2D& vOrigin = callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double rDist = callerAgent.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance();
    return MT_Sector( vOrigin, *direction, angle * MT_PI / 180. ).IsInCone( pKnowledge->GetPosition(), rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivedByAgent
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsPerceivedByAgent( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && callerAgent.GetKnowledge().IsPerceived( pKnowledge->GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsAnEnemy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::IsAnEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return static_cast< int >( eTristate_DontKnow );
    return static_cast< int >( pKnowledge->IsAnEnemy( callerAgent.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsAFriend
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::IsAFriend( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return static_cast< int >( eTristate_DontKnow );
    return static_cast< int >( pKnowledge->IsAFriend( callerAgent.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsMoving
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsMoving( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->GetSpeed() != 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceivingAgent
// Created: NLD 2004-05-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsPerceivingAgent( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    //$$$ Fonction BOF : trop de triche ...
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->GetAgentKnown().IsPerceived( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::KillOfficers
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::KillOfficers( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        pKnowledge->KillOfficers();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetOperationalState( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge )
        return float( pKnowledge->GetOperationalState() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMajorOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::GetMajorOperationalState( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetMajorOperationalState() == 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetPositionPtr
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgeAgentFunctions::GetPositionPtr( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > pos;
    if( pKnowledge.get() && pKnowledge->IsValid() )
        pos = pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
    return pos;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsFlying
// Created: JVT 2005-08-23
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsFlying( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->GetAltitude() > MIL_Tools::GetAltitude( pKnowledge->GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsKnowledgeValid
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsKnowledgeValid( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsPrisoner( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge&& pKnowledge->IsValid() && pKnowledge->IsPrisoner();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsDead
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsDead( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::Lock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::Lock( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        pKnowledge->Lock();
        return int( eQueryValid );
    }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::Unlock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::Unlock( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        pKnowledge->Unlock();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge
// Created: NLD 2005-05-17
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge( boost::shared_ptr< DEC_Knowledge_Agent > pSource, boost::shared_ptr< DEC_Knowledge_Agent > pTarget )
{
    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    if( pSource && pTarget && pSource->IsValid() && pTarget->IsValid() )
        return static_cast< float >( pSource->GetDangerosity( *pTarget ) + 1. );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosityOnPion
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetDangerosityOnPion( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const DEC_Decision_ABC* pTarget )
{
    assert( pTarget );
    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< float >( pKnowledge->GetDangerosity( pTarget->GetPion() ) + 1. );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetMaxPerceptionLevel().GetID() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool  DEC_KnowledgeAgentFunctions::IsIlluminated( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool  DEC_KnowledgeAgentFunctions::IsDefinitivelyIlluminated( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsDefinitevelyIlluminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::CanBeIlluminate
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::CanBeIlluminate( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    const dotation::PHY_RoleInterface_Dotations& roleDotations = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >();
    const float range = static_cast< float >( pKnowledge->GetPosition().Distance( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
    return roleDotations.GetIlluminationDotations( range, true ) || roleDotations.GetIlluminationDotations( range, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel
// Created: DDA 2010-04-27
// -----------------------------------------------------------------------------
double DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel( boost::shared_ptr< DEC_Knowledge_Agent > pTarget, boost::shared_ptr< DEC_Knowledge_Urban > pUrbanBlock )
{
    if( pUrbanBlock.get() && pUrbanBlock->IsValid() )
        return 1.f - pTarget->GetMaterialComposantesAttritionLevel( pUrbanBlock );
    throw std::exception( "urbanBlock not valid" );
}
