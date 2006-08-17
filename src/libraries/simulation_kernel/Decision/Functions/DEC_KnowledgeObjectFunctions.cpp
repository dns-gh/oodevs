// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeObjectFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 13:57 $
// $Revision: 9 $
// $Workfile: DEC_KnowledgeObjectFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KnowledgeObjectFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_SiteDecontamination.h"
#include "Entities/Objects/MIL_ZoneNBC.h"
#include "Entities/Objects/MIL_ItineraireLogistique.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObjectTypeFilter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::Recon( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    call.GetParameter( 1 ).SetValue( eQueryValid );
    pKnowledge->Recon( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::QueueForDecontamination
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::QueueForDecontamination( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() || pKnowledge->GetObjectKnown()->GetType() != MIL_RealObjectType::siteDecontamination_ )
    {
        call.GetResult().SetValue( eQueryInvalid );
        return;    
    }
    call.GetResult().SetValue( eQueryValid );
    static_cast< MIL_SiteDecontamination* >( pKnowledge->GetObjectKnown() )->QueueForDecontamination( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeAnimated
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeAnimated( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
    {
        call.GetResult().SetValue( false );
        return;    
    }
    bool bOut = pKnowledge->GetObjectKnown()->CanBeAnimatedBy( callerAgent );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DecontaminateZone( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    const TER_Localisation* pZoneDecontaminated = call.GetParameter( 0 ).ToUserPtr( pZoneDecontaminated );

    MIL_RealObjectTypeFilter objectsFilter;
    objectsFilter.Set( MIL_RealObjectType::zoneNBC_ );

    T_KnowledgeObjectVector knowledges;
    callerAgent.GetArmy().GetKnowledge().GetObjects( knowledges, objectsFilter );

    for( CIT_KnowledgeObjectVector it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        DEC_Knowledge_Object* pKnowledge = *it;
        if( !pKnowledge->GetObjectKnown() )
            continue;

        assert( pKnowledge->GetObjectKnown()->GetType() == MIL_RealObjectType::zoneNBC_ );
        static_cast< MIL_ZoneNBC* >( pKnowledge->GetObjectKnown() )->DecontaminateZone( *pZoneDecontaminated );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DamageObject
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DamageObject( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() || !pKnowledge->GetObjectKnown()->CanBePerceived() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        call.GetResult().SetValue( (int)0 );
        return;
    }

    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    call.GetParameter( 1 ).SetValue( eQueryValid );

    float rDamageFactor = call.GetParameter( 2 ).ToFloat();
    pObject->Destroy( rDamageFactor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeOccupied
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::CanBeOccupied( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
    {
        call.GetResult().SetValue( false );
        return;    
    }
    call.GetResult().SetValue( pKnowledge->GetObjectKnown()->CanBeOccupiedBy( callerAgent ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EquipLogisticRoute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::EquipLogisticRoute( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || pKnowledge->GetType() != MIL_RealObjectType::itineraireLogistique_ || !pKnowledge->GetObjectKnown() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }
    MIL_ItineraireLogistique& itiLog = static_cast< MIL_ItineraireLogistique& >( *pKnowledge->GetObjectKnown() );
    call.GetParameter( 1 ).SetValue( eQueryValid );
    itiLog.Equip();
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    call.GetParameter( 1 ).SetValue( eQueryValid );

    pObject->SetExitingPopulationDensity( call.GetParameter( 2 ).ToFloat() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    DEC_Knowledge_Object* pKnowledge = DEC_FunctionsTools::GetKnowledgeObjectFromDia( call.GetParameter( 0 ), callerAgent.GetArmy() );
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
    {
        call.GetParameter( 1 ).SetValue( eQueryInvalid );
        return;
    }

    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    call.GetParameter( 1 ).SetValue( eQueryValid );

    pObject->ResetExitingPopulationDensity();
}
