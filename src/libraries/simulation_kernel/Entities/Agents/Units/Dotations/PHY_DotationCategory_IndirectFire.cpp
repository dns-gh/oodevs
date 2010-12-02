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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/MIL_Army.h"
#include "Entities/Orders/MIL_Report.h"
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
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::Create
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_DotationCategory_IndirectFire::Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
{
    return *new PHY_DotationCategory_IndirectFire( type, dotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::PHY_DotationCategory_IndirectFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis )
    : PHY_DotationCategory_IndirectFire_ABC( type, dotationCategory, xis )
    , phs_( PHY_Posture::GetPostures().size(), 1. )
{
    xis >> xml::attribute( "neutralization-ratio", rNeutralizationCoef_ );
    if( rNeutralizationCoef_ < 1. )
        xis.error( "neutralization-ratio < 1" );
    if( !dotationCategory.HasAttritions() )
        xis.error( "Dotation has no attritions defined" );
    xis >> xml::list( "ph", *this, &PHY_DotationCategory_IndirectFire::ReadPh );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ReadPh
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ReadPh( xml::xistream& xis )
{
    const PHY_Posture::T_PostureMap& postures = PHY_Posture::GetPostures();
    std::string postureType;
    xis >> xml::attribute( "target-posture", postureType );
    PHY_Posture::CIT_PostureMap it = postures.find( postureType );
    const PHY_Posture& posture = *it->second;
    assert( phs_.size() > posture.GetID() );
    xis >> xml::attribute( "value", phs_[ posture.GetID() ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire::~PHY_DotationCategory_IndirectFire()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ApplyEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ApplyEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const
{
    MT_Vector2D vFireDirection        = ( vTargetPosition - vSourcePosition ).Normalize();
    MT_Vector2D vRotatedFireDirection = vFireDirection;
    vRotatedFireDirection.Rotate90();

    vFireDirection        *= ( rInterventionTypeFired * rDispersionX_ );
    vRotatedFireDirection *= ( rInterventionTypeFired * rDispersionY_ );

    // Agents
    {
        const MT_Ellipse attritionSurface     ( vTargetPosition, vTargetPosition + ( vFireDirection                        ),  vTargetPosition + ( vRotatedFireDirection                        ) );
        const MT_Ellipse neutralizationSurface( vTargetPosition, vTargetPosition + ( vFireDirection * rNeutralizationCoef_ ),  vTargetPosition + ( vRotatedFireDirection * rNeutralizationCoef_ ) );
        bool bRCSent = false;

        TER_Agent_ABC::T_AgentPtrVector targets;
        TER_World::GetWorld().GetAgentManager().GetListWithinEllipse( neutralizationSurface, targets );

        TER_Agent_ABC::T_AgentPtrVector allTargets;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle(vTargetPosition, 500., allTargets );
        
        std::vector< const urban::TerrainObject_ABC* > urbanList;
        if( MIL_AgentServer::IsInitialized() )
            MIL_AgentServer::GetWorkspace().GetUrbanModel().GetListWithinCircle( geometry::Point2f( static_cast< float >( vTargetPosition.rX_ ), static_cast< float >( vTargetPosition.rY_ ) ),
                                                                   static_cast< float >( rInterventionTypeFired * rDispersionX_ ), urbanList );
        
        for( TER_Agent_ABC::CIT_AgentPtrVector itAllTarget = allTargets.begin(); itAllTarget != allTargets.end(); ++itAllTarget )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itAllTarget ).GetAgent();
            const UrbanObjectWrapper* wrapper = target.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
            if( wrapper )
                if( std::find( targets.begin(), targets.end(), *itAllTarget ) == targets.end() || std::find( urbanList.begin(), urbanList.end(), &wrapper->GetObject() ) != urbanList.end() )
                    targets.push_back( *itAllTarget );
        }

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

        std::vector< TER_Object_ABC* > objects;
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vTargetPosition, rInterventionTypeFired * rDispersionX_ , objects );

        
        for( std::vector< TER_Object_ABC* >::iterator it = objects.begin(); it != objects.end(); ++it )
        {
            MIL_Object_ABC& object = *static_cast< MIL_Object_ABC* >( *it );
            StructuralCapacity* capacity = object.Retrieve< StructuralCapacity >();
            if( capacity )
                capacity->ApplyIndirectFire( object, attritionSurface, dotationCategory_ );
        }

        for( TER_Agent_ABC::CIT_AgentPtrVector itTarget = targets.begin(); itTarget != targets.end(); ++itTarget )
        {
            MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **itTarget ).GetAgent();
            if( target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 )
                continue;
            PHY_RoleInterface_ActiveProtection& targetRoleProtection = target.GetRole< PHY_RoleInterface_ActiveProtection >();
            bool counter = targetRoleProtection.CounterIndirectFire( dotationCategory_ );
            targetRoleProtection.UseAmmunition( dotationCategory_ );
            if( !counter )
            {
                PHY_RoleInterface_Composantes& targetRoleComposantes = target.GetRole< PHY_RoleInterface_Composantes >();
                targetRoleComposantes.Neutralize();
                float ratioComposanteHit = target.GetRole< PHY_RoleInterface_UrbanLocation >().ComputeRatioPionInside( attritionSurface );
                if( ratioComposanteHit > 0 )
                    targetRoleComposantes.ApplyIndirectFire( dotationCategory_, fireResult, ratioComposanteHit );

                if( !bRCSent && firer.GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
                {
                    MIL_Report::PostEvent( firer, MIL_Report::eReport_FratricideIndirectFire );
                    bRCSent = true;
                }
            }
        }
    }

    // Populations
    {
        const double  rAttritionRadius = std::min( vTargetPosition.Distance( vTargetPosition + vFireDirection ),
                                                     vTargetPosition.Distance( vTargetPosition + vRotatedFireDirection ) );
        const MT_Circle attritionCircle( vTargetPosition, rAttritionRadius );
        bool bRCSent = false;

        TER_PopulationConcentration_ABC::T_PopulationConcentrationVector concentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle ( vTargetPosition, rAttritionRadius, concentrations );
        for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
        {
            MIL_PopulationConcentration* pElement = static_cast< MIL_PopulationConcentration* >( *it );
            pElement->ApplyIndirectFire( attritionCircle, fireResult );
        }
        if( !concentrations.empty() && !bRCSent )
        {
            MIL_Report::PostEvent( firer, MIL_Report::eReport_IndirectFireOnPopulation );
            bRCSent = true;
        }

        TER_PopulationFlow_ABC::T_PopulationFlowVector flows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle ( vTargetPosition, rAttritionRadius, flows );
        for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector it = flows.begin(); it != flows.end(); ++it )
        {
            MIL_PopulationFlow* pElement = static_cast< MIL_PopulationFlow* >( *it );
            pElement->ApplyIndirectFire( attritionCircle, fireResult );
        }
        if( !flows.empty() && !bRCSent )
        {
            MIL_Report::PostEvent( firer, MIL_Report::eReport_IndirectFireOnPopulation );
            bRCSent = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory_IndirectFire::ApplyEffect
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
void PHY_DotationCategory_IndirectFire::ApplyEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, double /*rInterventionTypeFired*/, PHY_FireResults_ABC& fireResult ) const
{
    // Agents
    bool bRCSent = false;

    //Active Protection Management
    if( ! (target.GetRole< PHY_RoleInterface_Location >().GetHeight() > 0 ) )
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
            targetRoleComposantes.ApplyDirectFireOnMajorComposantes( dotationCategory_, fireResult );

            if( !bRCSent && firer.GetArmy().IsAFriend( target.GetArmy() ) == eTristate_True )
            {
                MIL_Report::PostEvent( firer, MIL_Report::eReport_FratricideIndirectFire );
                bRCSent = true;
            }
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

    const double rPH =   phs_[ targetPosture.GetCurrentPosture().GetID() ] *        targetPosture.GetPostureCompletionPercentage()
                         + phs_[ targetPosture.GetLastPosture   ().GetID() ] * ( 1. - targetPosture.GetPostureCompletionPercentage() );

    return ( 1. - MIL_Random::rand_io( MIL_Random::eFire ) ) <= (rPH * ratio ) ;
}
