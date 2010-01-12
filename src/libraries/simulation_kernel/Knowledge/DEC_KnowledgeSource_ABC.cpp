// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeSource_ABC.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeSource_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_KnowledgeBlackBoard_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::DEC_KnowledgeSource_ABC( DEC_KnowledgeBlackBoard_ABC& blackBoard, uint nPriority )
    : nPriority_  ( nPriority   )
    , pBlackBoard_( &blackBoard )
{
    blackBoard.AddToScheduler( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::DEC_KnowledgeSource_ABC()
    : nPriority_  ( 0 )
    , pBlackBoard_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::~DEC_KnowledgeSource_ABC()
{
    pBlackBoard_->RemoveFromScheduler( *this );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::GetPriority
// Created: NLD 2006-01-19
// -----------------------------------------------------------------------------
uint DEC_KnowledgeSource_ABC::GetPriority() const
{
    return nPriority_;
}
