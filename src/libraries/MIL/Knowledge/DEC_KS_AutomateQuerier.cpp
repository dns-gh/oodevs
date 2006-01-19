// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AutomateQuerier.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 16:12 $
// $Revision: 4 $
// $Workfile: DEC_KS_AutomateQuerier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_AutomateQuerier.h"

#include "DEC_Knowledge_RapForGLobal.h"
#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_KS_ArmyQuerier.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_KnowledgeGroupQuerier.h"

#include "Entities/Automates/MIL_Automate.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AutomateQuerier::DEC_KS_AutomateQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_Automate& automate )
    : DEC_KnowledgeSource_ABC( blackBoard, std::numeric_limits< uint >::max() )
    , pAutomate_             ( &automate )
{
        
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AutomateQuerier::~DEC_KS_AutomateQuerier()
{   
}

// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier::GetKnowledgeRapForGlobal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForGlobal& DEC_KS_AutomateQuerier::GetKnowledgeRapForGlobal() const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeRapForGlobal();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier::GetRapForGlobalValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
MT_Float DEC_KS_AutomateQuerier::GetRapForGlobalValue() const
{
	assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeRapForGlobal().GetValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier::GetObjects
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
void DEC_KS_AutomateQuerier::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    assert( pAutomate_ );
    pAutomate_->GetArmy().GetKSQuerier().GetObjects( container, filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
void DEC_KS_AutomateQuerier::GetFriendsInZone( T_KnowledgeAgentDiaIDVector&   container, const TER_Localisation& zone ) const
{
    assert( pAutomate_ );
    pAutomate_->GetKnowledgeGroup().GetKSQuerier().GetFriendsInZone( container, zone );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AutomateQuerier::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KS_AutomateQuerier::GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pAutomate_ );
    pAutomate_->GetKnowledgeGroup().GetKSQuerier().GetPopulations( container );
}
