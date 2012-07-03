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

#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/function.hpp>

class DEC_Knowledge_Population;
class MIL_PopulationAttitude;
class TerrainData;

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
             DEC_Path_KnowledgePopulation( const DEC_Knowledge_Population& knowledge, boost::function< double( unsigned int ) > populationAttitudeCost, bool avoidPolicy );
    virtual ~DEC_Path_KnowledgePopulation();
    //@}

    //! @name Operations
    //@{
    double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    void AddElement( const MIL_PopulationElement_ABC& element );
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
        TER_Localisation location_;
        double rDensity_;
        const MIL_PopulationAttitude* pAttitude_;
    };

    typedef std::vector< const sPopulationElement > T_PopulationElements;
    typedef T_PopulationElements::const_iterator  CIT_PopulationElements;
    //@}

    //! @name Helpers
    //@{
    double ComputeClosestElementInRange( const MT_Vector2D& position, double rMaxRange, sPopulationElement const*& pResult ) const;
    //@}

private:
    T_PopulationElements elements_;
    bool bAvoidPolicy_;
    boost::function< double( unsigned int ) > populationAttitudeCost_;
    double populationSecurityRange_;
    double costOutsideOfPopulation_;
};

#endif // __DEC_Path_KnowledgePopulation_h_
