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
#include <boost/noncopyable.hpp>

class DEC_Knowledge_Population;
class DEC_Agent_PathClass_ABC;
class MIL_PopulationAttitude;

// =============================================================================
/** @class  DEC_Path_KnowledgePopulation
    @brief  Population knowledge path data
*/
// Created: SBO 2006-02-23
// =============================================================================
class DEC_Path_KnowledgePopulation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgePopulation( const DEC_Knowledge_Population& knowledge, const DEC_Agent_PathClass_ABC& pathClass, bool avoidPolicy );
    virtual ~DEC_Path_KnowledgePopulation();
    //@}

    //! @name Operations
    //@{
    double ComputeCost( const MT_Vector2D& to ) const;
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
    //! @name 
    //@{
    T_PopulationElements elements_;
    const double rMaxRange_;
    const double rCostOutsideOfPopulation_;
    const std::vector< double > populationAttitudeCosts_;
    const bool bAvoidPolicy_;
    //@}
};

#endif // __DEC_Path_KnowledgePopulation_h_
