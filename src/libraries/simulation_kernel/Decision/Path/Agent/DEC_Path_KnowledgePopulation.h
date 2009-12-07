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
#include "Entities/Populations/MIL_PopulationElement_ABC.h"

class DEC_Agent_PathClass;
class DEC_Knowledge_Population;
class MIL_PopulationAttitude;
class TER_Localisation;

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
    DEC_Path_KnowledgePopulation( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Population& knowledge, bool avoidPolicy );
    ~DEC_Path_KnowledgePopulation();
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    void     AddElement ( const MIL_PopulationElement_ABC& element );
    //@}

private:
    //! @name Types
    //@{
    struct sPopulationElement
    {
    public:
        sPopulationElement( const MIL_PopulationElement_ABC& element )
            : location_  ( element.GetLocation() )
            , rDensity_  ( element.GetDensity() )
            , pAttitude_ ( &element.GetAttitude() ) 
        {}
    
              TER_Localisation        location_;
              MT_Float                rDensity_;
        const MIL_PopulationAttitude* pAttitude_;
    };

    typedef std::vector< const sPopulationElement >   T_PopulationElements;
    typedef T_PopulationElements::const_iterator    CIT_PopulationElements;
    //@}

    //! @name Helpers
    //@{
    MT_Float ComputeClosestElementInRange( const MT_Vector2D& position, MT_Float rMaxRange, sPopulationElement const*& pResult ) const;
    //@}

private:
          T_PopulationElements  elements_;
    const DEC_Agent_PathClass*  pPathClass_;
          bool                  bAvoidPolicy_;
};

#endif // __DEC_Path_KnowledgePopulation_h_
