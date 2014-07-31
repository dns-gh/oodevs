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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemy
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    return GetMaxRangeToFireOnEnemyWithDotation( callerAgent, pKnowledge, rWantedPH, nullptr );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation
// Created: LDC 2011-05-16
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH, const PHY_DotationCategory* dotation )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFireOn( *pKnowledge, rWantedPH, dotation, true ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemy
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToFireOnEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToFireOn( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFire( const MIL_Agent_ABC& callerAgent, float rWantedPH )
{
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFire( callerAgent, rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireDecision
// Created: DDA 2010-12-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireDecision( const DEC_Decision_ABC& callerAgent, float rWantedPH )
{
    return DEC_FireFunctions::GetMaxRangeToFire( callerAgent.GetPion(), rWantedPH );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToFireOnActualPosture( *pKnowledge, rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToFireOnActualPosture( *pKnowledge, rWantedPH );
    if( rRange == std::numeric_limits< double >::max() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( (float)rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
   return MIL_Tools::ConvertSimToMeter( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetOnlyLoadableMaxRangeToFireOn( *pKnowledge, rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( pKnowledge->GetMaxRangeToFireOn( callerAgent, rWantedPH ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    float rRange ( -1.f );
    if( pDotationCategory )
        rRange = ( float ) callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, true, false );
      if( rRange >= 0 )
          rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFire
// Created: GGE 2012-07-25
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFire( const MIL_AgentPion& callerAgent )
{
    float rRange ( -1.f );
    rRange = ( float ) callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire();
    if( rRange >= 0 )
        rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFireDecision
// Created: GGE 2013-04-22
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFireDecision( const DEC_Decision_ABC& callerAgent )
{
  return DEC_FireFunctions::GetMaxRangeToIndirectFire( callerAgent.GetPion());
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMinRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f ;
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, true, false );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return  -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    float rRange ( -1.f );
    if( pDotationCategory )
        rRange = ( float ) callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, true, true );
    if( rRange >= 0 )
        rRange = MIL_Tools::ConvertSimToMeter( rRange );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetTheoricMinRangeToIndirectFire
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetTheoricMinRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f ;
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, true, true );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return  -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
float DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f;
    float rRange = (float)callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMaxRangeToIndirectFire( *pDotationCategory, false, false );
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
float DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory )
{
    if( !pDotationCategory )
        return -1.f;
    const double rRange = callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMinRangeToIndirectFire( *pDotationCategory, false, false );
    if( rRange == std::numeric_limits< double >::max() ) // Pas de possibilité de tir
        return -1.f;
    return MIL_Tools::ConvertSimToMeter( rRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
double DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pTarget )
{
    if( pTarget && pTarget->IsValid() )
        return callerAgent.GetRole< PHY_RoleAction_IndirectFiring >().ThrowSmoke( pTarget->GetPosition() ); //$$$
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
const PHY_DotationCategory* DEC_FireFunctions::GetMunitionForIndirectFire( MIL_AgentPion& callerAgent, int indirectFireDotationClassID, const MT_Vector2D* pTarget )
{
    const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( indirectFireDotationClassID );
    assert( pClass );
    const PHY_DotationCategory* pDotationCategory = callerAgent.GetRole< PHY_RoleAction_IndirectFiring >().GetMunitionForIndirectFire( *pClass, pTarget );
    return pDotationCategory;
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::ForbidAmmunitions
// Created: HBD 2010-04-20
// -----------------------------------------------------------------------------
void DEC_FireFunctions::ForbidAmmunition( MIL_AgentPion& callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories )
{
    for( auto it = dotationCategories.begin(); it != dotationCategories.end(); ++it )
    {
        if( !*it )
            throw std::runtime_error( "Invalid pion in DEC_FireFunctions::ForbidAmmunition" );
        callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().SetForbiddenDotation( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::AllowAmmunition
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void DEC_FireFunctions::AllowAmmunition( MIL_AgentPion& callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories )
{
    for( auto it = dotationCategories.begin(); it != dotationCategories.end(); ++it )
    {
        if( !*it )
            throw std::runtime_error( "Invalid pion in DEC_FireFunctions::AllowAmmunition" );
        callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().RemoveForbiddenDotation( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::AllowAllAmmunitions
// Created: HBD 2010-04-22
// -----------------------------------------------------------------------------
void DEC_FireFunctions::AllowAllAmmunitions( MIL_AgentPion& callerAgent )
{
    callerAgent.GetRole< dotation::PHY_RoleInterface_Dotations >().AllowAllDotations();
}
