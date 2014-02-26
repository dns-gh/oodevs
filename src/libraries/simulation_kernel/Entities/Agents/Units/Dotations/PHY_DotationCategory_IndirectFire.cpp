// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationCategory_IndirectFire.h"
#include "PHY_DotationCategory.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Effects/MIL_Effect_Explosion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Objects/FireForbiddenCapacity.h"
#include "Entities/Objects/InfrastructureCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MIL_AgentServer.h"
#include "MIL_Random.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_Circle.h"
#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "Tools/MIL_Geometry.h"
#include "Urban/MIL_UrbanCache.h"
#include <boost/tuple/tuple.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                  unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
{
    return *new PHY_DotationCategory_IndirectFire( type, dotationCategory, xis, nInterventionType, rDispersionX, rDispersionY, rDetectionRange );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::PHY_DotationCategory_IndirectFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                      unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, nInterventionType, rDispersionX, rDispersionY, rDetectionRange )
    , rNeutralizationCoef_( xis.attribute< double >( "neutralization-ratio" ) )
    , rDispersionCoef_    ( xis.attribute< double >( "dispersion-factor", 0 ) )
    , phs_                ( PHY_Posture::GetPostureCount(), 1. )
{
    if( rNeutralizationCoef_ < 1. )
        throw MASA_EXCEPTION( xis.context() + "neutralization-ratio < 1" );
    if( !dotationCategory.HasAttritions() )
        throw MASA_EXCEPTION( xis.context() + "Dotation has no attritions defined" );
    xis >> xml::list( "ph", *this, &PHY_DotationCategory_IndirectFire::ReadPh );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ReadPh
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ReadPh( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "target-posture" );
    const PHY_Posture* posture = PHY_Posture::FindPosture( name );
    if( ! posture )
        throw MASA_EXCEPTION( xis.context() + "unknown posture name '" + name +"'" );
    assert( phs_.size() > posture->GetID() );
    phs_[ posture->GetID() ] = xis.attribute< double >( "value" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::~PHY_DotationCategory_IndirectFire()
{
    // NOTHING
}

namespace
{
    void AddAgentsInNearbyUrbanBlocks( TER_Agent_ABC::T_AgentPtrVector& targets, const MT_Vector2D& vTargetPosition, const MT_Ellipse& attritionSurface, double distanceEffectForBlocks )
    {
        TER_Agent_ABC::T_AgentPtrVector allTargets;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( vTargetPosition, distanceEffectForBlocks, allTargets );
        
        if( !allTargets.empty() )
        {
            std::vector< const MIL_UrbanObject_ABC* > urbanList;
            if( MIL_AgentServer::IsInitialized() )
                MIL_AgentServer::GetWorkspace().GetUrbanCache().GetListWithinCircle( vTargetPosition, static_cast< float >( distanceEffectForBlocks ), urbanList );
            if( urbanList.empty() )
                return;
            
            TER_Localisation explosionArea( attritionSurface );
            // explosionArea should be an ellipse, but since I don't have polygon/ellipse intersection code, I make do with a polygon.
            
            for( auto it = allTargets.begin(); it != allTargets.end(); ++it )
            {
                MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
                if( MIL_UrbanObject_ABC* wrapper = const_cast< MIL_UrbanObject_ABC* >( target.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock() ) )
                    if( std::find( targets.begin(), targets.end(), *it ) == targets.end() // Don't damage units twice
                        && std::find( urbanList.begin(), urbanList.end(), wrapper ) != urbanList.end() ) // Urban block of unit is in area of effect
                    {
                        const TER_Localisation& location = wrapper->GetLocalisation();
                        double intersection = MIL_Geometry::IntersectionArea( location, explosionArea );
                        if( 0 < intersection )
                            targets.push_back( *it );
    }
            }
        }

    }

    void DamageObjects( const MT_Vector2D& vTargetPosition, double distanceForObjects, const MT_Ellipse& attritionSurface, const PHY_DotationCategory& dotationCategory, const MIL_Agent_ABC* pFirer )
    {
        std::vector< TER_Object_ABC* > objects;
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vTargetPosition, distanceForObjects , objects );

        bool bFireInForbiddenArea = false;
        bool bInfrastructureDamaged = false;
        bool bLivingAreaDamaged = false;
        for( std::vector< TER_Object_ABC* >::iterator it = objects.begin(); it != objects.end(); ++it )
        {
            MIL_Object_ABC& obj = *static_cast< MIL_Object_ABC* >( *it );
            MIL_UrbanObject_ABC* wrapper = dynamic_cast< MIL_UrbanObject_ABC* >( &obj );
            float state = 0;
            if( wrapper )
               if( StructuralCapacity* structural = wrapper->Retrieve< StructuralCapacity>() )
                    state = structural->GetStructuralState();
            TER_Localisation polygon( attritionSurface );
            obj.ApplyIndirectFire( polygon, dotationCategory, pFirer ? &pFirer->GetArmy() : static_cast< MIL_Army_ABC* >( 0 ) );
            if( pFirer )
            {
                if( obj.Retrieve< FireForbiddenCapacity >() && !bFireInForbiddenArea && obj.GetArmy() && pFirer->GetArmy().IsAFriend( *obj.GetArmy() ) == eTristate_True )
                {
                    MIL_Report::PostEvent( *pFirer, report::eRC_TirDansZoneInterdite );
                    bFireInForbiddenArea = true;
                }
                if( wrapper )
                {
                    StructuralCapacity* structural = wrapper->Retrieve< StructuralCapacity>();
                    if( structural && structural->GetStructuralState() < state )
                    {
                        if( !bInfrastructureDamaged && wrapper->Retrieve< InfrastructureCapacity >() )
                        {
                            MIL_Report::PostEvent( *pFirer, report::eRC_InfrastructureDamaged );
                            bInfrastructureDamaged = true;
                        }
                        if( !bLivingAreaDamaged && wrapper->GetTotalInhabitants() > 0 )
                        {
                            MIL_Report::PostEvent( *pFirer, report::eRC_LivingAreaDamaged );
                            bLivingAreaDamaged = true;
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ApplyEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ApplyEffect( const MIL_Agent_ABC* pFirer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const
{
    MT_Vector2D vFireDirection = ( ( vTargetPosition == vSourcePosition ) ? MT_Vector2D( 1.f, 0.f ) : ( vTargetPosition - vSourcePosition ) ).Normalize() ;
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    const double itm1 = std::max( 0., rInterventionTypeFired - 1 );
    const double dispersionFactor = 1 + itm1 * rDispersionCoef_;
    const double phFactor = rInterventionTypeFired / dispersionFactor;

    vFireDirection        *= ( rDispersionX_ +  rDispersionX_ * itm1 * rDispersionCoef_ );
    vRotatedFireDirection *= ( rDispersionY_ +  rDispersionY_ * itm1 * rDispersionCoef_ );

    // Agents
    {
        const MT_Ellipse attritionSurface     ( vTargetPosition, vTargetPosition + ( vFireDirection                        ),  vTargetPosition + ( vRotatedFireDirection                        ) );
        const MT_Ellipse neutralizationSurface( vTargetPosition, vTargetPosition + ( vFireDirection * rNeutralizationCoef_ ),  vTargetPosition + ( vRotatedFireDirection * rNeutralizationCoef_ ) );

        // Area effect messages
        MIL_Effect_Explosion* attritionEffect = new MIL_Effect_Explosion( attritionSurface, category_, 20 , false );
        MIL_Effect_Explosion* neutralizationEffect = new MIL_Effect_Explosion( neutralizationSurface, category_, 20, true );
        MIL_EffectManager::GetEffectManager().Register( *attritionEffect );
        MIL_EffectManager::GetEffectManager().Register( *neutralizationEffect );

        std::vector< unsigned int > fireEffectsIds;
        fireEffectsIds.push_back( attritionEffect->GetFireEffectId() );
        fireEffectsIds.push_back( neutralizationEffect->GetFireEffectId() );
        ApplyDetectionRangeEffect( vTargetPosition, fireEffectsIds, 0 );

        TER_Agent_ABC::T_AgentPtrVector targets;
        TER_World::GetWorld().GetAgentManager().GetListWithinEllipse( neutralizationSurface, targets );

        double distanceEffectForBlocks = rInterventionTypeFired * ( rDispersionX_ + rDispersionY_ );
        AddAgentsInNearbyUrbanBlocks( targets, vTargetPosition, attritionSurface, distanceEffectForBlocks );

        for( TER_Agent_ABC::CIT_AgentPtrVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itTarget ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 )
                continue;
            PHY_RoleInterface_ActiveProtection& targetRoleProtection = target.GetRole< PHY_RoleInterface_ActiveProtection >();
            if( targetRoleProtection.DestroyIndirectFire( dotationCategory_ ) )
            {
                targetRoleProtection.UseAmmunition( dotationCategory_ );
                return;
            }
        }

        DamageObjects( vTargetPosition, rInterventionTypeFired * rDispersionX_, attritionSurface, dotationCategory_, pFirer );

        bool bRCSent = false;
        for( TER_Agent_ABC::CIT_AgentPtrVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itTarget ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 )
                continue;
            target.GetRole< PHY_RoleInterface_HumanFactors >().NotifyAttacked();
            PHY_RoleInterface_ActiveProtection& targetRoleProtection = target.GetRole< PHY_RoleInterface_ActiveProtection >();
            bool counter = targetRoleProtection.CounterIndirectFire( dotationCategory_ );
            targetRoleProtection.UseAmmunition( dotationCategory_ );
            if( !counter )
            {
                PHY_RoleInterface_Composantes& targetRoleComposantes = target.GetRole< PHY_RoleInterface_Composantes >();
                targetRoleComposantes.Neutralize();
                if( dotationCategory_.IsIED() )
                    MIL_Report::PostEvent( target, report::eRC_PrisSousTirIED );
                else
                    MIL_Report::PostEvent( target, report::eRC_PrisSousTirArtillerie );
                double ratioComposanteHit = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( attritionSurface );
                if( ratioComposanteHit > 0 )
                    targetRoleComposantes.ApplyIndirectFire( dotationCategory_, fireResult, ratioComposanteHit * phFactor );
                if( pFirer && !bRCSent && pFirer->GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True && !target.IsDead() )
                {
                    MIL_Report::PostEvent( *pFirer, report::eRC_TirIndirectFratricide );
                    bRCSent = true;
                }

                //check if EO close enough (< 5km) to observe fire effect
                TER_Agent_ABC::T_AgentPtrVector localObservers;
                TER_World::GetWorld().GetAgentManager().GetListWithinCircle ( vTargetPosition, 5000.0, localObservers );

                //RC
                for( TER_Agent_ABC::CIT_AgentPtrVector itobserver = localObservers.begin(); itobserver != localObservers.end(); ++itobserver )
                {
                    MIL_Agent_ABC& observerAgent = static_cast< PHY_RoleInterface_Location& >( **itobserver ).GetAgent();
                    //only take those observers belonging to same army as firer:
                    if( pFirer && observerAgent.GetArmy().GetID() != pFirer->GetArmy().GetID() )
                        continue;
                    PHY_RoleInterface_Perceiver& perceiver = observerAgent.GetRole< PHY_RoleInterface_Perceiver >();
                    if( perceiver.IsFireObserver() )
                    {
                        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = perceiver.GetKnowledgeGroup()->GetKnowledge()->GetKnowledgeAgentContainer().GetKnowledgeAgent( target );
                        if( knowledge && knowledge->GetRelevance() == 1. && knowledge->GetCurrentPerceptionLevel() >= PHY_PerceptionLevel::detected_ )
                        {
                            typedef std::vector< boost::tuple< std::string, unsigned int ,unsigned int, unsigned int > > T_Content;
                            T_Content content;
                            fireResult.GetDamages( target ).Serialize( content );
                            for( auto it = content.begin(); it != content.end(); ++it )
                                MIL_Report::PostEvent( observerAgent, report::eRC_FireObserver, target.GetID(),
                                                       it->get< 0 >(), it->get< 1 >(), it->get< 2 >(), it->get< 3 >() );
                        }
                    }
                }
            }
        }
    }

    // Populations
    {
        const double  rAttritionRadius = std::min( vTargetPosition.Distance( vTargetPosition + vFireDirection ),
                                                   vTargetPosition.Distance( vTargetPosition + vRotatedFireDirection ) );
        const MT_Circle attritionCircle( vTargetPosition, rAttritionRadius );

        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle ( vTargetPosition, rAttritionRadius, concentrations );
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
        {
            MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *it );
            pElement->ApplyIndirectFire( attritionCircle, fireResult );
        }

        TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle ( vTargetPosition, rAttritionRadius, flows );
        for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector it = flows.begin(); it != flows.end(); ++it )
        {
            MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *it );
            pElement->ApplyIndirectFire( attritionCircle, fireResult );
        }
        if( pFirer && ( !concentrations.empty() || !flows.empty() ) )
            MIL_Report::PostEvent( *pFirer, report::eRC_TirIndirectSurPopulation );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ApplyStrikeEffect
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ApplyStrikeEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const
{
    //Active Protection Management
    if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() <= 0 )
    {
        PHY_RoleInterface_ActiveProtection& targetRoleProtection = target.GetRole< PHY_RoleInterface_ActiveProtection >();
        if( targetRoleProtection.DestroyIndirectFire( dotationCategory_ ) )
        {
            targetRoleProtection.UseAmmunition( dotationCategory_ );
            return;
        }

        bool counter = targetRoleProtection.CounterIndirectFire( dotationCategory_ );
        targetRoleProtection.UseAmmunition( dotationCategory_ );
        if( !counter )
        {
            PHY_RoleInterface_Composantes& targetRoleComposantes = target.GetRole< PHY_RoleInterface_Composantes >();
            targetRoleComposantes.Neutralize();
            targetRoleComposantes.ApplyIndirectFire( dotationCategory_, fireResult, rInterventionTypeFired );

            if( firer.GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
                MIL_Report::PostEvent( firer, report::eRC_TirIndirectFratricide );
        }
    }
    // Precision Fire, no Population attrition
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::HasHit
// Created: NLD 2005-08-04
// -----------------------------------------------------------------------------
bool PHY_DotationCategory_IndirectFire::HasHit( const MIL_Agent_ABC& target, double ratio ) const
{
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();
    const double rPH = phs_[ targetPosture.GetCurrentPosture().GetID() ] *        targetPosture.GetPostureCompletionPercentage()
                     + phs_[ targetPosture.GetLastPosture   ().GetID() ] * ( 1. - targetPosture.GetPostureCompletionPercentage() );
    return ( 1. - MIL_Random::rand_io( MIL_Random::eFire ) ) <= ( rPH * ratio ) ;
}
