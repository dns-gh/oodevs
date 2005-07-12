// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_KnowledgeAgent.h $
// $Author: Age $
// $Modtime: 8/03/05 18:53 $
// $Revision: 6 $
// $Workfile: DEC_Path_KnowledgeAgent.h $
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeAgent_h_
#define __DEC_Path_KnowledgeAgent_h_

#include "MIL.h"

class DEC_Knowledge_Agent;
class DEC_Path;

// =============================================================================
// Created: NLD 2004-04-06
// =============================================================================
class DEC_Path_KnowledgeAgent
{

public:
    //! @name Constructors/Destructor
    //@{
     DEC_Path_KnowledgeAgent( const DEC_Knowledge_Agent& knowledge, const DEC_Path& path );
    ~DEC_Path_KnowledgeAgent();
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D vPosition_;
    //@}
};

#include "DEC_Path_KnowledgeAgent.inl"

#endif // __DEC_Path_KnowledgeAgent_h_
