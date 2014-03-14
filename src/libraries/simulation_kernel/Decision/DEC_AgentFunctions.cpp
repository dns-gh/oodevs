// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AgentFunctions.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 15:50 $
// $Revision: 37 $
// $Workfile: DEC_AgentFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AgentFunctions.h"
#include "DEC_AutomateFunctions.h"
#include "DEC_GeometryFunctions.h"
#include "DotationComputer_ABC.h"
#include "OnComponentComputer_ABC.h"
#include "Decision/DEC_Gen_Object.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Humans/PHY_NbcSuit.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Dotations/PHY_Dotation.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationGroupContainer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "Entities/Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Effects/MIL_Effect_Suicide.h"
#include "Entities/Specialisations/LOG/Medical/MIL_AgentTypePionLOGMedical.h"
#include "Entities/Objects/ActivableCapacity.h"
#include "Entities/Objects/BypassAttribute.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "MIL_AgentServer.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "meteo/PHY_Precipitation.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Specialisations/NBC/MIL_AgentTypePionNBC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_AgentManager.h"

DEC_Decision_ABC* DEC_AgentFunctions::GetUnitById( uint32_t id )
{
    auto unit = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( id );
    if( !unit )
        return nullptr;
    return &unit->GetRole< DEC_RolePion_Decision >();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsNeutralized
// Created: JVT 03-10-01
//-----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsNeutralized( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsNeutralized();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsUnderIndirectFire
// Created: LMT 2012-06-13
//-----------------------------------------------------------------------------
bool DEC_AgentFunctions::UpdateUnderIndirectFire( MIL_Agent_ABC& callerAgent )
{
    return callerAgent.UpdateUnderIndirectFire();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::WearNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::WearNbcProtectionSuit( MIL_Agent_ABC& callerAgent )
{
    if( GetNbcSuitLevel( callerAgent) != 0 )
        callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().WearNbcProtectionSuit();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RemoveNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RemoveNbcProtectionSuit( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().RemoveNbcProtectionSuit();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SelfDecontaminate
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SelfDecontaminate( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().Decontaminate();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ActivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ActivateBlackout( MIL_Agent_ABC& callerAgent )
{
    callerAgent.CallRole( &PHY_RoleInterface_Communications::ActivateBlackout );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ActivatePartialBlackout
// Created: HBD 2010-06-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ActivatePartialBlackout( MIL_Agent_ABC& callerAgent, bool report )
{
    callerAgent.CallRole( &PHY_RoleInterface_Communications::ActivatePartialBlackout, report );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DeactivateBlackout
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DeactivateBlackout( MIL_Agent_ABC& callerAgent )
{
    callerAgent.CallRole( &PHY_RoleInterface_Communications::DeactivateBlackout );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsAgentNBC
// Created: DDA 2012-04-12
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsAgentNBC( const MIL_Agent_ABC& callerAgent )
{
    return dynamic_cast< const MIL_AgentTypePionNBC* >( &callerAgent.GetType() ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsContaminated( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsIntoxicated
// Created: LGY 2012-01-13
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsIntoxicated( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().IsIntoxicated();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TemporaryImmunizeAgent
// Created: GGE 2012-05-14
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::TemporaryImmunizeAgent( DEC_Decision_ABC* pAgent, bool bImmunize )
{
    pAgent->GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().TemporaryImmunizeAgent( bImmunize );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsTransported
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsTransported( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().IsTransported();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetFlyingHeight( MIL_Agent_ABC& callerAgent, double height )
{
    callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().SetFlyingHeight( height );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsFlying( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsDead
// Created: NLD 2006-02-01
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsDead( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsPC
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsPC( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.IsPC();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanPerformHealthEvacuation
// Created: JSR 2012-04-18
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanPerformHealthEvacuation( const DEC_Decision_ABC* agent )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in DEC_AgentFunctions::CanPerformHealthEvacuation" );

    const PHY_RolePion_Composantes& pComposantes = agent->GetPion().GetRole< PHY_RolePion_Composantes >();
    return pComposantes.CanEvacuateCasualties();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsAutomateEngaged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsAutomateEngaged( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetAutomate().IsEngaged();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetOperationalState
// Created: NLD 2004-04-16
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetOperationalState( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetMajorOperationalState( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosition
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetPosition( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetNbcSuitLevel
// Created: LGY 2012-11-27
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetNbcSuitLevel( const MIL_Agent_ABC& callerAgent )
{
    return static_cast< int >( callerAgent.GetType().GetUnitType().GetNbcSuit().GetType() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentPositionPtr
// Created: DDA 2010-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetAgentPositionPtr( DEC_Decision_ABC* brain )
{
    if( !brain )
        throw MASA_EXCEPTION( "Invalid parameter passed to DEC_AgentFunctions::GetAgentPositionPtr" );
    return brain->GetPion().GetRole< PHY_RoleInterface_Location >().GetSharedPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDirection
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
const MT_Vector2D* DEC_AgentFunctions::GetDirection( const MIL_Agent_ABC& callerAgent )
{
    return &callerAgent.GetRole< PHY_RoleInterface_Location >().GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetIdentificationDistance
// Created: DDA 2010-03-24
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetIdentificationDistance( MIL_Agent_ABC& callerAgent )
{
    const PHY_Composante_ABC* majorComposante = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComposante();
    if( !majorComposante )
        throw MASA_EXCEPTION( "Unit has no major component" );
    return majorComposante->GetIdentificationMaxRange();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetReconnoissanceDistance
// Created: GGE & PSN 2010-04-20
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetReconnoissanceDistance( MIL_Agent_ABC& callerAgent )
{
    const PHY_Composante_ABC* majorComposante = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComposante();
    if( !majorComposante )
        throw MASA_EXCEPTION( "Unit has no major component" );
    return majorComposante->GetReconnoissanceMaxRange();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDetectionDistance
// Created: DDA 2010-06-09
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetDetectionDistance( MIL_Agent_ABC& callerAgent )
{
    return (float)callerAgent.GetRole< PHY_RoleInterface_Perceiver >().GetMaxAgentPerceptionDistance();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return CanConstructObjectWithLocalisation( callerAgent, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObjectWithLocalisation
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructObjectWithLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type, location, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructWithoutReinforcement
// Created: LGY 2011-10-04
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructWithoutReinforcement( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return CanConstructWithoutReinforcementWithLocalisation( callerAgent, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructWithoutReinforcementWithLocalisation
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructWithoutReinforcementWithLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().CanConstructWithoutReinforcement( type, location, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObjectWithLoaded
// Created: MGD 2010-11-02
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructObjectWithLoaded( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return CanConstructObjectWithLoadedAndLocalisation( callerAgent, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanConstructObjectWithLoadedAndLocalisation
// Created: JSR 2012-02-24
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanConstructObjectWithLoadedAndLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type, location, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotationForBuilding
// Created: LMT 2010-07-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotationForBuilding( MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().EnoughDotationForBuilding( type, callerAgent, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotationForBuildingWithoutReinforcement
// Created: LMT 2012-01-24
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotationForBuildingWithoutReinforcement( MIL_Agent_ABC& callerAgent, const std::string& type )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().EnoughDotationForBuilding( type, callerAgent, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanBypassObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanBypassObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && objectKnowledge->RetrieveAttribute< BypassAttribute >() != 0 && callerAgent.GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanDestroyObject
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanDestroyObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanDemineObject
// Created: DDA 2012-03-16
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanDemineObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanDemineWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanMineObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanMineObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && objectKnowledge->IsConstructed() && callerAgent.GetRole< PHY_RoleAction_Objects >().CanMineWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanActivateObject
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanActivateObject( boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    return objectKnowledge && objectKnowledge->IsValid() && objectKnowledge->GetType().GetCapacity< ActivableCapacity >();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableDiscreteMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().EnableDiscreteMode();
}

//-----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableDiscreteMode
// Created: JVT 04-05-17
//-----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableDiscreteMode( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Posture >().DisableDiscreteMode();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanFly
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanFly( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetType().GetUnitType().CanFly();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetStandardFlyingHeight
// Created: JSR 2013-10-18
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetStandardFlyingHeight( const DEC_Decision_ABC* callerAgent )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "Invalid pion in DEC_AgentFunctions::GetStandardFlyingHeight" );
    return callerAgent->GetPion().GetType().GetUnitType().GetStandardFlyingHeight();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetTacticalFlyingHeight
// Created: JSR 2013-10-18
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetTacticalFlyingHeight( const DEC_Decision_ABC* callerAgent )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "Invalid pion in DEC_AgentFunctions::GetTacticalFlyingHeight" );
    return callerAgent->GetPion().GetType().GetUnitType().GetTacticalFlyingHeight();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DecisionalState( const MIL_Agent_ABC& callerAgent, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_unit()->set_id( callerAgent.GetID() );
    msg().set_key( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsLoaded
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsLoaded( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Loading >().IsLoaded();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsUnloaded
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsUnloaded( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleAction_Loading >().IsUnloaded();
}

namespace
{
    class HasLoadableComputer : public OnComponentComputer_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
        HasLoadableComputer()
            : bHasLoadable_( false )
        {
            // NOTHING
        }
        virtual ~HasLoadableComputer()
        {
            // NOTHING
        }
        //@}

        //! @name Operations
        //@{
        virtual void ApplyOnComponent( PHY_ComposantePion& component )
        {
            if( component.IsLoadable() )
                bHasLoadable_ = true;
        }

        bool HasLoadable()
        {
            return bHasLoadable_;
        }
        //@}

    private :
        //! @name Member data
        //@{
        bool bHasLoadable_;
        //@}
    };
}
// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasLoadable
// Created: MGD 2010-08-30
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasLoadable( const MIL_Agent_ABC& callerAgent )
{
    HasLoadableComputer computer;
    const_cast< MIL_Agent_ABC& >( callerAgent ).Execute< OnComponentComputer_ABC >( computer );
    return computer.HasLoadable();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetLoadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetLoadingTime( const MIL_Agent_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetLoadingTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetUnloadingTime
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetUnloadingTime( const MIL_Agent_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetUnloadingTime() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableHumanTransportersNow( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().DisableHumanTransporters( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RecoverHumanTransportersNow
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::RecoverHumanTransportersNow( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().RecoverHumanTransporters();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AreHumanTransportersReady
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AreHumanTransportersReady( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< transport::PHY_RoleInterface_Transported >().HasHumanTransportersReady();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanMount
// Created: LMT 2011-05-18
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanMount( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().Get< PHY_RoleInterface_UrbanLocation >().CanMount() && !callerAgent.GetPion().Get< PHY_RoleAction_MovingUnderground >().IsUnderground();
}
// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::PionCanFly
// Created: MIA 2013-06-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::PionCanFly( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent && callerAgent->GetPion().GetType().GetUnitType().CanFly();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyForceRatioStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyForceRatioStateChanged( static_cast< E_ForceRatioState >( state ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyIndirectFireAvailabilityChanged( static_cast< E_FireAvailability >( state ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyRulesOfEngagementStateChanged( static_cast< E_RulesOfEngagementState >( state ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetRulesOfEngagementState
// Created: DDA 2010-02-03
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetRulesOfEngagementState( const MIL_Agent_ABC& callerAgent )
{
   return static_cast< int >( callerAgent.GetRole< DEC_RolePion_Decision >().GetRulesOfEngagementState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged
// Created: SBO 2005-11-22
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged( MIL_Agent_ABC& callerAgent, int population )
{
    const PHY_RoePopulation* pRoe = PHY_RoePopulation::Find( population );
    if( !pRoe )
        throw MASA_EXCEPTION( "Invalid RoE" );
    callerAgent.GetRole< DEC_RolePion_Decision >().NotifyRoePopulationChanged( *pRoe );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyOperationalStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyOperationalStateChanged( static_cast< E_OperationalState >( state ) );
}

// -----------------------------------------------------------------------------
// Name: static void DEC_AgentFunctions::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::NotifyCloseCombatStateChanged( MIL_Agent_ABC& callerAgent, int state )
{
    dynamic_cast< DEC_RolePion_Decision& >( callerAgent.GetDecision() ).NotifyCloseCombatStateChanged( static_cast< E_CloseCombatState >( state ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorTiredness
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetHumanFactorTiredness( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_HumanFactors >().GetTiredness().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetHumanFactorMorale
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
unsigned int DEC_AgentFunctions::GetHumanFactorMorale( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_HumanFactors >().GetMorale().GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::RelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* agentToRelieve )
{
    if( !agentToRelieve || !callerAgent.GetOrderManager().RelievePion( agentToRelieve->GetPion() ) )
        return false;
    agentToRelieve->GetPion().GetOrderManager().CancelMission();
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanRelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* pAgentToRelieve )
{
    if( !pAgentToRelieve )
        throw MASA_EXCEPTION( "Invalid pion in DEC_AgentFunctions::CanRelievePion" );
    return callerAgent.GetOrderManager().CanRelievePion( pAgentToRelieve->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SurrenderAutomat
// Created: MMC 2013-03-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::SurrenderAutomat( MIL_Agent_ABC& callerAgent, DEC_Decision_ABC* automat )
{
    if( !automat )
        throw MASA_EXCEPTION( "Invalid automat in DEC_AgentFunctions::SurrenderAutomat" );
    if( automat->GetAutomate().GetArmy() == callerAgent.GetArmy() )
        return false;
    if( automat->GetAutomate().IsSurrendered() )
        return false;
    automat->GetAutomate().SurrenderWithUnits( callerAgent.GetArmy() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionsWithPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithoutPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionsWithoutPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetCommunicationPionsWithPC
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetCommunicationPionsWithPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetCommunicationPionsWithPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetCommunicationPionsWithoutPC
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetCommunicationPionsWithoutPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetCommunicationPionsWithoutPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPC
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AgentFunctions::GetPionPC( const MIL_Agent_ABC& callerAgent )
{
    return DEC_AutomateFunctions::GetPionPC( callerAgent.GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionPCOfAutomate
// Created: JVT 2005-01-18
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_AgentFunctions::GetPionPCOfAutomate( const DEC_Decision_ABC* automat )
{
    if( !automat )
        throw MASA_EXCEPTION( "Invalid automat in DEC_AgentFunctions::GetPionPCOfAutomate" );
    return DEC_AutomateFunctions::GetPionPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPionsWithoutPCOfAutomate
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::GetPionsWithoutPCOfAutomate( const DEC_Decision_ABC* automat )
{
    if( !automat )
        throw MASA_EXCEPTION( "Invalid automat in DEC_AgentFunctions::GetPionsWithoutPCOfAutomate" );
    return DEC_AutomateFunctions::GetPionsWithoutPC( automat->GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ChangeAutomate
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::ChangeAutomate( MIL_Agent_ABC& callerAgent, DEC_Decision_ABC* automat )
{
    if( !automat )
        throw MASA_EXCEPTION( "Invalid automat in DEC_AgentFunctions::" );
    if( automat->GetAutomate().GetArmy() == callerAgent.GetArmy() )
    {
        callerAgent.ChangeSuperior( automat->GetAutomate() );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetPosture
// Created: JVT 2005-01-31
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetPosture( const MIL_Agent_ABC& callerAgent )
{
    const PHY_RoleInterface_Posture& rolePosture = callerAgent.GetRole< PHY_RoleInterface_Posture >();
    return static_cast< int >( rolePosture.GetPostureCompletionPercentage() >= 1. ? rolePosture.GetCurrentPosture().GetID() : rolePosture.GetLastPosture().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInCity
// Created: MGD 2010-05-11
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInCity( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().IsInCity();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInCrowd
// Created: DDA 2011-05-13
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInCrowd( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.Get< PHY_RoleInterface_Population >().HasCollision();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeLeftForMoving
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::TimeLeftForMoving( const MIL_Agent_ABC& callerAgent )
{
    const double rTime = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().GetMaxTimeForConsumption( PHY_ConsumptionType::moving_ );
    return static_cast< float >( MIL_Tools::ConvertSimToMinutes( rTime ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::TimeToMoveDistance
// Created: JVT 2005-02-07
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::TimeToMoveDistance( const MIL_Agent_ABC& callerAgent, float distance )
{
   const double rMaxSpeed = callerAgent.GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetMaxSpeed();
   if( rMaxSpeed == 0 )
       return std::numeric_limits< float >::max();
    return static_cast< float >( MIL_Tools::ConvertSimToMinutes( MIL_Tools::ConvertMeterToSim( distance ) / rMaxSpeed ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetInterceptionPoint
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetInterceptionPoint( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > result;
    if( pKnowledge && pKnowledge->IsValid())
    {
        result.reset( new MT_Vector2D() );
        bool valid = DEC_GeometryFunctions::GetInterceptionPoint( pKnowledge->GetPosition(), pKnowledge->GetDirection() * pKnowledge->GetSpeed(),
                callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(),
                callerAgent.GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetMaxSpeed(), *result );
        if( !valid )
            result.reset();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetRoePopulation
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
int DEC_AgentFunctions::GetRoePopulation( const MIL_Agent_ABC& callerAgent )
{
    return static_cast< int >( callerAgent.GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ChannelPopulation
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ChannelPopulations( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in ChannelPopulation" );
    MIL_AgentServer::GetWorkspace().GetEntityManager().ChannelPopulations( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AlertInhabitants
// Created: BCI 2011-02-01
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::AlertInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in AlertInhabitants" );
    MIL_AgentServer::GetWorkspace().GetEntityManager().AlertInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::UndoAlertInhabitants
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::UndoAlertInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in UndoAlertInhabitants" );
    MIL_AgentServer::GetWorkspace().GetEntityManager().UndoAlertInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInhabitantsAlerted
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInhabitantsAlerted( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in IsInhabitantsAlerted" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().IsInhabitantsAlerted( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ConfineInhabitants
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ConfineInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in ConfineInhabitants" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().ConfineInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::UndoConfineInhabitants
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::UndoConfineInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in UndoConfineInhabitants" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().UndoConfineInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInhabitantsConfined
// Created: CCD 2012-01-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInhabitantsConfined( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in IsInhabitantsConfined" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().IsInhabitantsConfined( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::UrbanBlockIsPopulated
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::UrbanBlockIsPopulated( const MIL_UrbanObject_ABC* pUrbanBlock )
{
     if ( pUrbanBlock )
        return pUrbanBlock->GetTotalInhabitants() > 0;
     return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EvacuateInhabitants
// Created: CCD 2012-01-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EvacuateInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in EvacuateInhabitants" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().EvacuateInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::UndoEvacuateInhabitants
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::UndoEvacuateInhabitants( const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location in UndoEvacuateInhabitants" );
    return MIL_AgentServer::GetWorkspace().GetEntityManager().UndoEvacuateInhabitants( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInhabitantsEvacuated
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInhabitantsEvacuated( const TER_Localisation* location )
{
    return MIL_AgentServer::GetWorkspace().GetEntityManager().IsInhabitantsEvacuated( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInstalled( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Deployment >().IsDeployed();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsUninstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsUninstalled( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Install
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Install( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Deployment >().StartDeploy();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetInstallationTime
// Created: JSR 2011-11-07
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetInstallationTime( DEC_Decision_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetPion().GetType().GetUnitType().GetInstallationTime() );   // $$$$ ABR 2011-12-15: Old method, should be removed soon
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetUninstallationTime
// Created: JSR 2011-11-07
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetUninstallationTime( DEC_Decision_ABC& callerAgent )
{
    return MIL_Tools::ConvertSimToMinutes( callerAgent.GetPion().GetType().GetUnitType().GetUninstallationTime() ); // $$$$ ABR 2011-12-15: Old method, should be removed soon
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsDeployed
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsDeployed( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Deployment >().IsDeployed();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsUndeployed
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsUndeployed( const MIL_Agent_ABC& callerAgent )
{
    return callerAgent.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Deploy
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Deploy( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Deployment >().Deploy();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Undeploy
// Created: ABR 2011-12-15
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Undeploy( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< PHY_RoleInterface_Deployment >().Undeploy();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDirectionDanger
// Created: LDC 2009-04-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_AgentFunctions::GetDirectionDanger( const boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    if( !pMission.get() )
        return boost::shared_ptr< MT_Vector2D >();
    return pMission->GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMission
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_Mission_ABC > DEC_AgentFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw MASA_EXCEPTION( "Invalid pion for GetMission" );
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasMission
// Created: LDC 2010-06-23
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasMission( DEC_Decision_ABC* pAgent )
{
    return ( pAgent && 0 != pAgent->GetMission().get() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetDotation
// Created: BAX 2014-03-04
// -----------------------------------------------------------------------------
const PHY_DotationCategory* DEC_AgentFunctions::GetDotation( const MIL_Agent_ABC& caller, unsigned id )
{
    const PHY_DotationCategory* reply = nullptr;
    caller.GetRole< dotation::PHY_RoleInterface_Dotations >().Apply( [&]( PHY_Dotation& dotation )
    {
        if( reply )
            return;
        const auto& cat = dotation.GetCategory();
        if( id == cat.GetMosID() )
            reply = &cat;
    });
    return reply;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotation
// Created: LDC 2010-03-26
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotation( const MIL_Agent_ABC& callerAgent, const PHY_DotationCategory* category )
{
    return category ? callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().GetDotationNumber( *category ) > 0 : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::HasDotationForFiring
// Created: LDC 2011-06-23
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::HasDotationForFiring( DEC_Decision_ABC* agent, const PHY_DotationCategory* category, int iterations )
{
    return agent && category && agent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().HasDotationForFiring( *category , iterations );
}

namespace
{
    class CanUseDotationComputer : public dotation::DotationComputer_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
        CanUseDotationComputer( const PHY_DotationCategory* category, bool& result ) : category_( category ), result_( result ) {}
        virtual ~CanUseDotationComputer() {}
        //@}

        //! @name Operations
        //@{
        virtual void SetDotationContainer( PHY_DotationGroupContainer& /*container*/ ) {}
        virtual bool HasDotation( const PHY_DotationCategory& /*category*/ ) const { return false; }
        virtual double GetDotationValue( const PHY_DotationCategory& /*category*/ ) const { return 0.; }
        virtual double GetDotationCapacity( const PHY_DotationCategory& /*category*/ ) const { return 0.; }
        virtual void SetForbiddenAmmunition( const std::vector< const PHY_DotationCategory* >* container )
        {
            if( container )
                for( std::vector< const PHY_DotationCategory* >::const_iterator it = container->begin(); it != container->end(); ++it )
                    if( category_ == *it )
                    {
                        result_ = false;
                        return;
                    }
        }
        //@}
    private:
        bool& result_;
        const PHY_DotationCategory* category_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanUseDotation
// Created: HBD 2010-10-01
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanUseDotation( MIL_Agent_ABC& callerAgent, const PHY_DotationCategory* category )
{
    bool result = true;
    CanUseDotationComputer dotationComputer( category, result );
    callerAgent.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetFuelDotationNumber
// Created: JSR 2012-07-31
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetFuelDotationNumber(  MIL_AgentPion& /*callerAgent*/, const DEC_Decision_ABC* pPion  )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Invalid pion in GetFuelDotationNumber" );
    return pPion->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().GetFuelDotationNumber();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::Suicide
// Created: LDC 2010-03-29
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::Suicide( MIL_Agent_ABC& callerAgent )
{
    MIL_Effect_Suicide* pEffect = new MIL_Effect_Suicide( callerAgent );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CanBeIlluminate
// Created: GGE 2010-06-17
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CanIlluminate( DEC_Decision_ABC* pAgent )
{
    if( pAgent )
    {
        const dotation::PHY_RoleInterface_Dotations& roleDotations = pAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >();
        if( roleDotations.GetIlluminationDotations( 0.0, true ) || roleDotations.GetIlluminationDotations( 0.0, false ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float DEC_AgentFunctions::GetIlluminatingRange( const MIL_Agent_ABC& callerAgent )
{
    const dotation::PHY_RoleInterface_Dotations& roleDotations = callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >();
    return roleDotations.GetIlluminatingRange( );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsImmobilized
// Created: LDC 2011-08-04
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsImmobilized( DEC_Decision_ABC* pAgent )
{
    return pAgent && pAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().IsImmobilized();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisableCrowdEffect
// Created: JSR 2012-04-18
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisableCrowdEffect( const DEC_Decision_ABC* pAgent, bool disable )
{
    if( pAgent )
        pAgent->GetPion().GetRole< PHY_RoleInterface_Population >().DisableCrowdEffect( disable );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsJammed
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsJammed( DEC_Decision_ABC* pAgent )
{
    return pAgent && pAgent->GetPion().CallRole( &PHY_RolePion_Communications::IsJammed, false);
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInEmissionBlackout
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInEmissionBlackout( DEC_Decision_ABC* pAgent )
{
    return pAgent && pAgent->GetPion().CallRole( &PHY_RolePion_Communications::IsInEmissionBlackout, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInReceptionBlackout
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInReceptionBlackout( DEC_Decision_ABC* pAgent )
{
    return pAgent && pAgent->GetPion().CallRole( &PHY_RolePion_Communications::IsInReceptionBlackout, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::SetToAmbianceSafety
// Created: MMC 2013-04-19
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::SetToAmbianceSafety( MIL_Agent_ABC& callerAgent, bool safety )
{
    callerAgent.GetRole< PHY_RolePion_Posture >().SetAmbianceSafety( safety );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IsInSmoke
// Created: JSR 2013-06-03
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::IsInSmoke( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        return false;
    const MT_Vector2D position = pAgent->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    const auto& data = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();
    const auto id = data.GetPrecipitation( position ).GetID();
    return id == weather::PHY_Precipitation::smoke_.GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::IdentifyAllAgentsInZone
// Created: JSR 2011-05-05
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::IdentifyAllAgentsInZone( MIL_Agent_ABC& callerAgent, const TER_Localisation* location )
{
    if( !location )
        MT_LOG_ERROR_MSG( "Identifying agent in nil zone" );
    TER_Agent_ABC::T_AgentPtrVector agentsDetected;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( *location, agentsDetected );
    PHY_RoleInterface_Perceiver& perceiver = callerAgent.GetRole< PHY_RoleInterface_Perceiver >();
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsDetected.begin(); itAgent != agentsDetected.end(); ++itAgent )
        perceiver.NotifyPerception( static_cast< PHY_RoleInterface_Location& >( **itAgent ).GetAgent(), PHY_PerceptionLevel::identified_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMilPionType
// Created: DDA 2011-05-23
// -----------------------------------------------------------------------------
std::string DEC_AgentFunctions::GetMilPionType( DEC_Decision_ABC* pion )
{
    if( !pion )
        throw MASA_EXCEPTION( "Invalid pion in GetMilPionType" );
    return pion->GetPion().GetType().GetMilPionType();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMilPionName
// Created: PSN 2012-08-16
// -----------------------------------------------------------------------------
std::string DEC_AgentFunctions::GetMilPionName( DEC_Decision_ABC* pion )
{
    if( !pion )
        throw MASA_EXCEPTION( "Invalid pion in GetMilPionName" );
    return pion->GetPion().GetType().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetRapForLocalAgent
// Created: DDA 2011-06-14
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetRapForLocalAgent( const DEC_Decision_ABC* agent )
{
    return agent ? agent->GetPion().GetKnowledge().GetRapForLocalValue() : 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetCurrentSpeed
// Created: LMT 2011-07-05
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetCurrentSpeed( const DEC_Decision_ABC* agent )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in GetCurrentSpeed" );
    const PHY_RoleInterface_Location& roleLocation = agent->GetPion().GetRole< PHY_RoleInterface_Location >();
    return MIL_Tools::ConvertSecondsToSim( roleLocation.GetCurrentSpeed() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanConstructObjectWithLoaded
// Created: LMT 2011-08-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanConstructObjectWithLoaded( const DEC_Decision_ABC* agent, const std::string& type )
{
    return AgentCanConstructObjectWithLoadedAndLocalisation( agent, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanConstructObjectWithLoadedAndLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanConstructObjectWithLoadedAndLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanConstructObjectWithLoaded" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type, localisation, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToBuild
// Created: LMT 2012-01-31
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToBuild( const std::vector< DEC_Decision_ABC* >& units, const std::string& type )
{
    return RetrieveUnitsAbleToBuildWithLocalisation( units, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToBuildWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToBuildWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToBuild;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanConstructObjectWithOutLoadedWithLocalisation( *it, type, localisation ) )
            unitsAbleToBuild.push_back( *it );
    return unitsAbleToBuild;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToMine
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToMine( const std::vector< DEC_Decision_ABC* >& units, const std::string& type )
{
    return RetrieveUnitsAbleToMineWithLocalisation( units, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToMineWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToMineWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToMine;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanMineObject( *it, type, localisation ) )
            unitsAbleToMine.push_back( *it );
    return unitsAbleToMine;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithLocalisation
// Created: LMT 2013-03-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToDemine;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanDemineObject( *it, type, localisation ) )
            unitsAbleToDemine.push_back( *it );
    return unitsAbleToDemine;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithOutLoadedWithLocalisation
// Created: GGE 2013-05-21
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithOutLoadedWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToMine;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanDemineObjecttWithOutLoaded( *it, type, localisation ) )
            unitsAbleToMine.push_back( *it );
    return unitsAbleToMine;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToByPass
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToByPass( const std::vector< DEC_Decision_ABC* >& units, const std::string& type )
{
    return RetrieveUnitsAbleToByPassWithLocalisation( units, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToByPassWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToByPassWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToByPass;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanByPassObjectWithLocalisation( *it, type, localisation ) )
            unitsAbleToByPass.push_back( *it );
    return unitsAbleToByPass;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::RetrieveUnitsAbleToDestroy
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToDestroy( const std::vector< DEC_Decision_ABC* >& units, const std::string& type )
{
    return RetrieveUnitsAbleToDestroyWithLocalisation( units, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToDestroyWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AgentFunctions::RetrieveUnitsAbleToDestroyWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation )
{
    std::vector< DEC_Decision_ABC* > unitsAbleToDestroy;
    std::vector< DEC_Decision_ABC* >::const_iterator it;
    for( it = units.begin(); it != units.end(); ++it )
        if( AgentCanDestroyObjectTypeWithLocalisation( *it, type, localisation ) ) //TODO a changer
            unitsAbleToDestroy.push_back( *it );
    return unitsAbleToDestroy;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanExtinguish
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanExtinguish( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return callerAgent.GetRole< PHY_RoleAction_Objects >().CanExtinguish( pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentHasDotationForBuilding
// Created: LMT 2011-08-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentHasDotationForBuilding( const DEC_Decision_ABC* agent, const std::string& type )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentHasDotationForBuilding" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().EnoughDotationForBuilding( type, agent->GetPion(), true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentDotationNumber
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetAgentDotationNumber( const DEC_Decision_ABC* agent, const PHY_DotationCategory* dotationCategory )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in GetAgentDotationForBuilding" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().GetAgentDotationNumber( agent->GetPion(), dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentMissingDotationForBuildingObject
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > DEC_AgentFunctions::GetAgentMissingDotationForBuildingObject( const DEC_Decision_ABC* agent, const DEC_Gen_Object* object )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in GetAgentMissingDotationForBuildingObject" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().GetAgentMissingDotationForBuildingObject( object, agent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentMissingDotationForBuildingExistingObject
// Created: DDA 2012-04-05
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > DEC_AgentFunctions::GetAgentMissingDotationForBuildingExistingObject( const DEC_Decision_ABC* agent, const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in GetAgentMissingDotationForBuildingExistingObject" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().GetAgentMissingDotationForBuildingExistingObject( pKnowledge, agent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetAgentMissingDotationForMining
// Created: LMT 2012-02-03
// -----------------------------------------------------------------------------
std::pair< const PHY_DotationCategory*, double > DEC_AgentFunctions::GetAgentMissingDotationForMining( const DEC_Decision_ABC* agent, const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in GetAgentMissingDotationForMining" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().GetAgentMissingDotationForMiningObstacle( pKnowledge, agent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanConstructObjectWithOutLoadedWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanConstructObjectWithOutLoadedWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanConstructObjectWithOutLoadedWithLocalisation" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanConstructWithReinforcement( type, localisation, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanMineObject
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanMineObject( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanMineObject" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanMineTypeWithReinforcement( type, localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanDemineObject
// Created: LMT 2013-03-27
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanDemineObject( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw std::runtime_error( "Invalid pion in AgentCanDemineObject" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanDemineTypeWithReinforcement( type, localisation, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanDemineObjecttWithOutLoaded
// Created: GGE 2013-05-21
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanDemineObjecttWithOutLoaded( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw std::runtime_error( "Invalid pion in AgentCanDemineObjecttWithOutLoaded" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanDemineTypeWithReinforcement( type, localisation, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanByPassObjectWithLocalisation
// Created: LMT 2012-02-01
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanByPassObjectWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanUnmineObject" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassTypeWithReinforcement( type, localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentHasDotationForBuildingWithOutLoaded
// Created: LMT 2012-01-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentHasDotationForBuildingWithOutLoaded( const DEC_Decision_ABC* agent, const std::string& type )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentHasDotationForBuildingWithOutLoaded" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().EnoughDotationForBuilding( type, agent->GetPion(), false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanDestroyObject
// Created: LMT 2011-08-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanDestroyObject( const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanDestroyObject" );
    return objectKnowledge && objectKnowledge->IsValid() && agent && agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanDestroyObjectType
// Created: LMT 2011-08-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanDestroyObjectType( const DEC_Decision_ABC* agent, const std::string& type )
{
    return AgentCanDestroyObjectTypeWithLocalisation( agent, type, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanDestroyObjectTypeWithLocalisation
// Created: JSR 2012-02-27
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanDestroyObjectTypeWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanDestroyObjectType" );
    return agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanDestroyTypeWithReinforcement( type, localisation );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentCanBypassObject
// Created: LMT 2011-08-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentCanBypassObject( const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentCanBypassObject" );
    return objectKnowledge && objectKnowledge->IsValid() && objectKnowledge->RetrieveAttribute< BypassAttribute >() != 0 && agent && agent->GetPion().GetRole< PHY_RoleAction_Objects >().CanBypassWithReinforcement( objectKnowledge->GetType(), objectKnowledge->GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentHasRadar
// Created: DDA 2011-12-19
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentHasRadar( const DEC_Decision_ABC* agent, int typeRadar )
{
    if( !agent )
        throw MASA_EXCEPTION( "Invalid pion in AgentHasTappingRadar" );
    PHY_RoleInterface_Perceiver& agentPerceiver = agent->GetPion().GetRole< PHY_RoleInterface_Perceiver >();
    const PHY_RadarClass* radar = PHY_RadarClass::Find( typeRadar );
    if( !radar )
        throw MASA_EXCEPTION( "Invalid radar in AgentHasTappingRadar" );
    return !agentPerceiver.GetRadars( *radar ).empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::AgentHasFuseau
// Created: GGE 2010-03-25
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::AgentHasFuseau(const MIL_Agent_ABC& callerAgent )
{
    return !callerAgent.GetOrderManager().GetFuseau().IsNull();
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CreateInstantaneously
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::CreateInstantaneously( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Gen_Object > pGenObject )
{
    if( !callerAgent || !pGenObject )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_Object_ABC* object = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( &callerAgent->GetPion().GetArmy(), pGenObject->GetTypeName(), &pGenObject->GetLocalisation(), pGenObject->GetActivated(), pGenObject->GetExternalIdentifier(), pGenObject->GetName() );
    if( !object )
        throw MASA_EXCEPTION( "unable to create object." );
    object->Initialize( *pGenObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::ImmunizeAgent
// Created: LGY 2013-01-14
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::ImmunizeAgent( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().TemporaryImmunizeAgent( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::StopImmunizeAgent
// Created: LGY 2013-01-14
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::StopImmunizeAgent( MIL_Agent_ABC& callerAgent )
{
    callerAgent.GetRole< nbc::PHY_RoleInterface_NBC >().TemporaryImmunizeAgent( false );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::GetMaxSpeed
// Created: NMI 2013-10-11
// -----------------------------------------------------------------------------
double DEC_AgentFunctions::GetMaxSpeed( const DEC_Decision_ABC& agent )
{
    const moving::PHY_RoleAction_InterfaceMoving& roleMoving = agent.GetPion().GetRole< moving::PHY_RoleAction_InterfaceMoving >();
    return MIL_Tools::ConvertSecondsToSim( roleMoving.GetMaxSpeed() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::CreateBreakdown
// Created: LGY 2014-02-26
// -----------------------------------------------------------------------------
bool DEC_AgentFunctions::CreateBreakdown( MIL_Agent_ABC& callerAgent, unsigned int type, unsigned int breakdown )
{
    const PHY_BreakdownType* breakdownType = PHY_BreakdownType::Find( breakdown );
    const PHY_ComposanteTypePion* composanteType = PHY_ComposanteTypePion::Find( type );
    if( breakdownType && composanteType )
        return callerAgent.GetRole< PHY_RolePion_Composantes >().
            CreateBreakdowns( *composanteType, 1, breakdownType );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableSharedPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableSharedPerception( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in EnableSharedPerception" );
    pAgent->GetPion().GetKnowledgeGroup()->RegisterSharingPerceptions( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisabledSharedPerception
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisabledSharedPerception( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in DisabledSharedPerception" );
    pAgent->GetPion().GetKnowledgeGroup()->UnregisterSharingPerceptions( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableSharingKnowledges
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableSharingKnowledges( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in EnableSharedPerception" );
    callerAgent.GetKnowledgeGroup()->RegisterSharingKnowledges( pAgent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisabledSharingKnowledges
// Created: LGY 2013-05-07
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisabledSharingKnowledges( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in DisabledSharedPerception" );
    callerAgent.GetKnowledgeGroup()->UnregisterSharingKnowledges( pAgent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableSharedPerceptionWithKnowledge
// Created: GGE 2013-06-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid())
        pKnowledge->GetAgentKnown().GetKnowledgeGroup()->RegisterSharingPerceptions( callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisabledSharedPerceptionWithKnowledge
// Created: GGE 2013-06-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisabledSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid())
        pKnowledge->GetAgentKnown().GetKnowledgeGroup()->UnregisterSharingPerceptions( callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::EnableSharingKnowledgesWithKnowledge
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::EnableSharingKnowledgesWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid())
        pKnowledge->GetAgentKnown().GetKnowledgeGroup()->RegisterSharingKnowledges( callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::DisabledSharingKnowledgesWithKnowledge
// Created: LGY 2013-09-09
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::DisabledSharingKnowledgesWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid())
        pKnowledge->GetAgentKnown().GetKnowledgeGroup()->UnregisterSharingKnowledges( callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::KnowledgeEnableSharedPerceptionWithKnowledge
// Created: GGE 2013-06-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::KnowledgeEnableSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in KnowledgeEnableSharedPerceptionWithKnowledge" );

    pAgent->GetPion().GetKnowledgeGroup()->RegisterSharingPerceptions( callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AgentFunctions::KnowledgeDisabledSharedPerceptionWithKnowledge
// Created: GGE 2013-06-03
// -----------------------------------------------------------------------------
void DEC_AgentFunctions::KnowledgeDisabledSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in KnowledgeDisabledSharedPerceptionWithKnowledge" );
    pAgent->GetPion().GetKnowledgeGroup()->UnregisterSharingPerceptions( callerAgent.GetPion() );
}

bool DEC_AgentFunctions::KnowledgeCommunicate( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid pion in KnowledgeCommunicate" );
    return pAgent->GetPion().GetKnowledgeGroup()->GetId() == callerAgent.GetKnowledgeGroup()->GetId();
}
