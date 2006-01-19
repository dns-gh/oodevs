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

#include "MIL_pch.h"
#include "DEC_KnowledgeSource_ABC.h"


#include "DEC_KnowledgeBlackBoard.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeSource_ABC, "DEC_KnowledgeSource_ABC" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::DEC_KnowledgeSource_ABC( DEC_KnowledgeBlackBoard& blackBoard, uint nPriority )
    : pBlackBoard_( &blackBoard )
    , nPriority_  ( nPriority   )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::DEC_KnowledgeSource_ABC()
    : pBlackBoard_( 0 )
    , nPriority_  ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeSource_ABC::~DEC_KnowledgeSource_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeSource_ABC::Talk()
{
    // Nothing
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KnowledgeSource_ABC::Prepare()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::Clean
// Created: NLD 2005-09-01
// -----------------------------------------------------------------------------
void DEC_KnowledgeSource_ABC::Clean()
{
    // Nothing
}
