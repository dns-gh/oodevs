// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeRapFor.cpp $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeRapFor.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeRapFor.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeRapFor, "DEC_BlackBoard_CanContainKnowledgeRapFor" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeRapFor::DEC_BlackBoard_CanContainKnowledgeRapFor()
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeRapFor::~DEC_BlackBoard_CanContainKnowledgeRapFor()
{
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeRapFor::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> knowledgeRapForLocal_;
    file >> knowledgeRapForGlobal_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeRapFor::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeRapFor::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeRapForLocal_;
    file << knowledgeRapForGlobal_;
}
