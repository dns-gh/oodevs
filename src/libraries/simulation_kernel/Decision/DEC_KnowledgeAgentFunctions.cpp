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
#include "DEC_KnowledgeFunctions.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Knowledge/QueryValidity.h"
#include "protocol/ClientSenders.h"

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
        return static_cast< float >( pKnowledge->GetDangerosity( callerAgent, false, 0 ) + 1.f );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetPotentialAttrition
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetPotentialAttrition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< MT_Vector2D > position )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< float >( callerAgent.GetDangerosity( pKnowledge, position, false, 0 ) );
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
    if( !direction )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MT_Vector2D& vOrigin = callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const double rDist = callerAgent.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance();
    return MT_Sector( vOrigin, *direction, angle * MT_PI / 180. ).IsInCone( pKnowledge->GetPosition(), rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInCrowd
// Created: DDA 2011-05-13
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsInCrowd( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    return pKnowledge->IsInCrowd();
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
        pos = pKnowledge->GetPositionPtr();
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
// Name: DEC_KnowledgeAgentFunctions::DEC_ConnaissanceAgent_Altitude
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
double DEC_KnowledgeAgentFunctions::DEC_ConnaissanceAgent_Altitude( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if ( !pKnowledge )
        return 0.;
    return pKnowledge->GetAltitude();
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
// Name: DEC_KnowledgeAgentFunctions::IsPrisoner
// Created: DDA 2012-03-27
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsTerrorist( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge&& pKnowledge->IsValid() && pKnowledge->IsTerrorist();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsSurrenderToMyArmy
// Created: EVH 2011-05-16
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsSurrenderToMyArmy( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        const MIL_Army_ABC* surrenderTo = pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().GetArmySurrenderedTo();
        if( surrenderTo )
            return &callerAgent.GetPion().GetArmy() == surrenderTo;
    }
    return false;
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
// Name: DEC_KnowledgeAgentFunctions::IsSurrendered
// Created: NLD 2011-05-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsSurrendered( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsSurrendered();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::Lock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::Lock( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
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
    if( pKnowledge && pKnowledge->IsValid() )
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
        return static_cast< float >( pSource->GetDangerosity( *pTarget, false, 0 ) + 1. );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetDangerosityOnPion
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeAgentFunctions::GetDangerosityOnPion( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const DEC_Decision_ABC* pTarget )
{
    if( !pTarget )
        throw MASA_EXCEPTION( "invalid parameter." );
    // For DIA, the dangerosity value is 1 <= dangerosity <= 2
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< float >( pKnowledge->GetDangerosity( pTarget->GetPion(), false, 0 ) + 1. );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetMaxPerceptionLevel().GetID() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
int DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsPerceptionLevelMax
// Created: JSR 2014-08-06
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsPerceptionLevelMax( const MIL_Entity_ABC& /*caller*/, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    // For compatibility with Scipio decisional.
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetCurrentPerceptionLevel() == PHY_PerceptionLevel::identified_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool  DEC_KnowledgeAgentFunctions::IsIlluminated( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsIlluminated
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool  DEC_KnowledgeAgentFunctions::IsDefinitivelyIlluminated( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsDefinitevelyIlluminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::CanBeIlluminate
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::CanBeIlluminate( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    const dotation::PHY_RoleInterface_Dotations& roleDotations = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >();
    const float range = static_cast< float >( pKnowledge->GetPosition().Distance( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() ) );
    return roleDotations.GetIlluminationDotations( range, true ) || roleDotations.GetIlluminationDotations( range, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::CanIlluminate
// Created: MGD 2010-12-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::CanIlluminate( const MIL_AgentPion& callerAgent )
{
    const dotation::PHY_RoleInterface_Dotations& roleDotations = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >();
    return roleDotations.GetIlluminationDotations( 0, true ) || roleDotations.GetIlluminationDotations( 0, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel
// Created: DDA 2010-04-27
// -----------------------------------------------------------------------------
double DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel( boost::shared_ptr< DEC_Knowledge_Agent > pTarget, MIL_UrbanObject_ABC* pUrbanBlock )
{
    if( pUrbanBlock && pTarget )
        return pTarget->GetMaterialComposantesAttritionLevel( pUrbanBlock );
    throw MASA_EXCEPTION( "urbanBlock or target not valid" );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
const std::string DEC_KnowledgeAgentFunctions::GetCriticalIntelligence( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    pKnowledge->SetCriticalIntelligenceFromAgentKnown();
    return pKnowledge->GetCriticalIntelligence();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::UnitDecisionalState
// Created: EVH 2011-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::UnitDecisionalState( const DEC_Knowledge_Agent& callerAgent, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_unit()->set_id( callerAgent.GetAgentKnown().GetID() );
    msg().set_key( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
    if( key == "hostage" && value == "true" && callerAgent.IsValid() )
        callerAgent.GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().SetAsPrisoner();
    if( key == "hostage" && value == "false" && callerAgent.IsValid() )
        callerAgent.GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().SetAsFree();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::DisableCrowdEffect
// Created: JSR 2012-04-18
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::DisableCrowdEffect( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool disable )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Population >().DisableCrowdEffect( disable );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge
// Created: EVH 2011-05-12
// -----------------------------------------------------------------------------
unsigned int DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsRefugee
// Created: EVH 2011-05-16
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsRefugee( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetType().IsRefugee();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsTransported
// Created: DDA 2012-01-10
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsTransported( const MIL_AgentPion& /*callerAgent*/, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->IsTransported();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetMilPionType
// Created: DDA 2011-05-23
// -----------------------------------------------------------------------------
std::string DEC_KnowledgeAgentFunctions::GetMilPionType( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetType().GetMilPionType();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MIL_Mission_ABC > DEC_KnowledgeAgentFunctions::GetMission
// Created: EVH 2011-05-24
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_KnowledgeAgentFunctions::GetMission( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pKnowledge->GetAgentKnown().GetDecision().GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::NeutralizeAgent
// Created: LMT 2011-05-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::NeutralizeAgent( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    PHY_RolePion_Composantes* role = pKnowledge->GetAgentKnown().RetrieveRole< PHY_RolePion_Composantes >();
    if( role )
        role->DestroyAllComposantes();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::ForceRadioSilence
// Created: EVH 2011-06-01
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::ForceRadioSilence( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool silence )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    PHY_RoleInterface_Communications* role = pKnowledge->GetAgentKnown().RetrieveRole< PHY_RoleInterface_Communications >();
    if( role )
        if( silence )
            role->ActivateBlackout  ();
        else
            role->DeactivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::ShareKnowledgesWith
// Created: EVH 2011-08-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::ShareKnowledgesWith( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float minutes )
{
    if( pKnowledge )
        DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_AgentPion >( callerAgent.GetPion(), &( pKnowledge->GetAgentKnown().GetDecision() ), minutes );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::GetAgent
// Created: DDA 2011-10-11
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_KnowledgeAgentFunctions::GetAgent( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge )
        return &pKnowledge->GetAgentKnown().GetDecision();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::DisableAutomateLogistic
// Created: DDA 2011-10-11
// -----------------------------------------------------------------------------
void DEC_KnowledgeAgentFunctions::SwitchAutomateLogistic( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge )
    {
        logistic::LogisticHierarchy_ABC* logHierarchy = &pKnowledge->GetAgentKnown().GetAutomate().GetLogisticHierarchy();
        if( logHierarchy )
            logHierarchy->SwitchToHierarchy( callerAgent.GetAutomate().GetLogisticHierarchy() );
    }
}

namespace
{
    template< typename T > bool IsAgentKnowledgeInObject( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, int isFriend, T& typeChecker )
    {
        // The test on isDead is to allow the use of GetObjectsColliding - otherwise it finds invalid knowledge and a bad result
        // A better solution would be to check for all objects at the location but it's expensive and the function is not needed for dead units anyway.
        if( pKnowledge && pKnowledge->IsValid() && !pKnowledge->GetAgentKnown().IsDead() )
        {
            T_KnowledgeObjectDiaIDVector objectsColliding;
            pKnowledge->GetAgentKnown().GetKnowledge().GetObjectsColliding( objectsColliding );
            for( auto it = objectsColliding.begin(); it != objectsColliding.end(); ++it )
            {
                const auto object = *it;
                const MIL_ObjectType_ABC& type = (*it)->GetType();
                unsigned int id = object->GetObjectId();
                boost::shared_ptr< DEC_Knowledge_Object > callerObjectKnowledge;
                if( !callerAgent.GetKnowledgeGroup()->IsJammed() )
                    callerObjectKnowledge = callerAgent.GetArmy().GetKnowledge().ResolveKnowledgeObjectByObjectID( id );
                if( !callerObjectKnowledge.get() && callerAgent.GetKnowledgeGroup() )
                {
                    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
                    if( bbKg )
                        callerObjectKnowledge = bbKg->ResolveKnowledgeObjectByObjectID( id );
                }
                if( !callerObjectKnowledge || !callerObjectKnowledge->IsValid() )
                    continue;
                if( typeChecker( type ) && static_cast< int >( object->IsAnEnemy( callerAgent.GetArmy() ) ) == isFriend )
                    return true;
            }
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInObject
// Created: JSR 2012-01-26
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsInObject( const MIL_AgentPion& callerAgent, const std::string& objectType, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, int isFriend )
{
    auto func = [&]( const MIL_ObjectType_ABC& type ) { return type.GetName() == objectType; };
    return IsAgentKnowledgeInObject( callerAgent, pKnowledge, isFriend, func );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeAgentFunctions::IsInObjectWithCapacity
// Created: LDC 2014-08-05
// -----------------------------------------------------------------------------
bool DEC_KnowledgeAgentFunctions::IsInObjectWithCapacity( const MIL_AgentPion& callerAgent, const std::string& capacity, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, int isFriend )
{
    auto func = [&]( const MIL_ObjectType_ABC& type ) { return CapacityRetriever::RetrieveCapacity( type, capacity ) != 0; };
    return IsAgentKnowledgeInObject( callerAgent, pKnowledge, isFriend, func );
}
