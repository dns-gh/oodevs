// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 16:22 $
// $Revision: 9 $
// $Workfile: DEC_FireFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_FireFunctions.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"

using namespace firing;

void DEC_FireFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxPourTirerSurUnite", &DEC_FireFunctions::GetMaxRangeToFireOnEnemy );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition", &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation );
    brain.RegisterFunction( "_DEC_Tir_PorteeMinPourTirerSurUnite", &DEC_FireFunctions::GetMinRangeToFireOnEnemy );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles", &DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture );
    brain.RegisterFunction( "_DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles", &DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite", &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxPourEtreTireParUnite", &DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourTirer", &DEC_FireFunctions::GetMaxRangeToFire );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxTirIndirect", static_cast< float(*)( const DEC_Decision_ABC*, const PHY_DotationCategory*) >( &DEC_FireFunctions::GetMaxRangeToIndirectFire ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition", static_cast< float(*)( const DEC_Decision_ABC* ) >( &DEC_FireFunctions::GetMaxRangeToIndirectFire ) );
    brain.RegisterFunction( "_DEC_Tir_PorteeMinTirIndirect", &DEC_FireFunctions::GetMinRangeToIndirectFire );
    brain.RegisterFunction( "_DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions", &DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck );
    brain.RegisterFunction( "_DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions",  &DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck );
    brain.RegisterFunction( "_DEC_Tir_PorteeTheoriqueMaxTirIndirect", &DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire );
    brain.RegisterFunction( "_DEC_Tir_PorteeTheoriqueMinTirIndirect", &DEC_FireFunctions::GetTheoricMinRangeToIndirectFire );
    brain.RegisterFunction( "_DEC_Tir_LancerFumigeneSurConnaissance", &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent ); // $$$$ MCO 2012-06-25: should be an action
    brain.RegisterFunction( "DEC_Tir_LancerFumigeneSurPosition", &DEC_FireFunctions::ThrowSmokeOnPosition );
    brain.RegisterFunction( "_DEC_Tir_MunitionPourTirIndirect", &DEC_FireFunctions::GetMunitionForIndirectFire );
    brain.RegisterFunction( "_DEC_Pion_InterdireMunition", &DEC_FireFunctions::ForbidAmmunition );
    brain.RegisterFunction( "_DEC_Pion_AutoriserMunition", &DEC_FireFunctions::AllowAmmunition );
    brain.RegisterFunction( "_DEC_Pion_AutoriserToutesMunitions", &DEC_FireFunctions::AllowAllAmmunitions );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemy
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    return GetMaxRangeToFireOnEnemyWithDotation( callerAgent, pKnowledge, rWantedPH, nullptr );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation
// Created: LDC 2011-05-16
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH, const PHY_DotationCategory* dotation )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFireOn( *pKnowledge, rWantedPH, dotation, true ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemy
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToFireOnEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToFireOn( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFire( const DEC_Decision_ABC* callerAgent, float rWantedPH )
{
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFire( callerAgent->GetPion(), rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFireOnActualPosture( *pKnowledge, rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToFireOnActualPosture( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( (float)rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
   return MIL_Tools::ConvertSimToMeter( callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetOnlyLoadableMaxRangeToFireOn( *pKnowledge, rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( pKnowledge->GetMaxRangeToFireOn( callerAgent->GetPion(), rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    float rRange ( -1.f );
    if( pDotationCategory )
        rRange = ( float ) callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, true, false );
      if( rRange >= 0 )
          rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFire
// Created: GGE 2012-07-25
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent )
{
    float rRange ( -1.f );
    rRange = ( float ) callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire();
    if( rRange >= 0 )
        rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f ;
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, true, false );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return  -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    float rRange ( -1.f );
    if( pDotationCategory )
        rRange = ( float ) callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, true, true );
    if( rRange >= 0 )
        rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetTheoricMinRangeToIndirectFire
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetTheoricMinRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f ;
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, true, true );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return  -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f;
    float rRange = (float)callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, false, false );
    if( rRange < 0. ) // Pas de possibilité de tir
        return -1.f;
    if( pDotationCategory->IsGuided() )
        rRange = std::min( rRange, pDotationCategory->GetGuidanceRange() );
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f;
    const double rRange = callerAgent->GetPion().GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, false, false );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
double DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pTarget )
{
    if( pTarget && pTarget->IsValid() )
        return callerAgent->GetPion().GetRole< PHY_RoleAction_IndirectFiring >().ThrowSmoke( pTarget->GetPosition() ); //$$$
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ThrowSmokeOnPosition
// Created: LDC 2013-08-29
// -----------------------------------------------------------------------------
double DEC_FireFunctions::ThrowSmokeOnPosition( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > pTarget )
{
    if( pTarget )
        return callerAgent.GetPion().GetRole< PHY_RoleAction_IndirectFiring >().ThrowSmoke( *pTarget );
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMunitionForIndirectFire
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
const PHY_DotationCategory* DEC_FireFunctions::GetMunitionForIndirectFire( DEC_Decision_ABC* callerAgent, int indirectFireDotationClassID, const MT_Vector2D* pTarget )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( indirectFireDotationClassID );
    assert( pClass );
    const PHY_DotationCategory* pDotationCategory = callerAgent->GetPion().GetRole< PHY_RoleAction_IndirectFiring >().GetMunitionForIndirectFire( *pClass, pTarget );
    return pDotationCategory;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ForbidAmmunitions
// Created: HBD 2010-04-20
// -----------------------------------------------------------------------------
void DEC_FireFunctions::ForbidAmmunition( DEC_Decision_ABC* callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories )
{
    for( auto it = dotationCategories.begin(); it != dotationCategories.end(); ++it )
    {
        if( !*it )
            throw std::runtime_error( "Invalid pion in DEC_FireFunctions::ForbidAmmunition" );
        callerAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().SetForbiddenDotation( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::AllowAmmunition
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void DEC_FireFunctions::AllowAmmunition( DEC_Decision_ABC* callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories )
{
    for( auto it = dotationCategories.begin(); it != dotationCategories.end(); ++it )
    {
        if( !*it )
            throw std::runtime_error( "Invalid pion in DEC_FireFunctions::AllowAmmunition" );
        callerAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().RemoveForbiddenDotation( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::AllowAllAmmunitions
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void DEC_FireFunctions::AllowAllAmmunitions( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< dotation::PHY_RoleInterface_Dotations >().AllowAllDotations();
}
