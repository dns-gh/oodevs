// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgePopulation_h_
#define __DEC_Path_KnowledgePopulation_h_

#include "MIL.h"

class DEC_Knowledge_Population;
class DEC_PathClass;

// =============================================================================
/** @class  DEC_Path_KnowledgePopulation
    @brief  Population knowledge path data
*/
// Created: SBO 2006-02-23
// =============================================================================
class DEC_Path_KnowledgePopulation
{

public:
    //! @name Constructors/Destructor
    //@{
    DEC_Path_KnowledgePopulation( const DEC_PathClass& pathClass, const DEC_Knowledge_Population& knowledge );
    ~DEC_Path_KnowledgePopulation();
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    const DEC_Knowledge_Population* knowledge_;
    const DEC_PathClass*            pathClass_;
};

#endif // __DEC_Path_KnowledgePopulation_h_
