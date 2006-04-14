// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Automate.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_Automate.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KnowledgeBlackBoard_Automate.h"

#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard_Automate, "DEC_KnowledgeBlackBoard_Automate" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate( MIL_Automate& automate )
    : pAutomate_            ( &automate )
    , knowledgeRapForGlobal_()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::DEC_KnowledgeBlackBoard_Automate()
    : pAutomate_            ( 0 )
    , knowledgeRapForGlobal_()
{
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Automate::~DEC_KnowledgeBlackBoard_Automate()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_Automate::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeBlackBoard_Automate::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pAutomate_
            & knowledgeRapForGlobal_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Automate::SendFullState() const
{
//    knowledgeRapForGlobal_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Automate::SendChangedState() const
{
//    knowledgeRapForGlobal_
}

