// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PopulationContext_h_
#define __DEC_PopulationContext_h_

#include "DEC_PopulationContext_ABC.h"

class MIL_Population;
class MIL_PopulationType;

// =============================================================================
/** @class  DEC_PopulationContext
    @brief  Provides path related context for the pathfinder
*/
// Created: MCO 2014-05-14
// =============================================================================
class DEC_PopulationContext : public DEC_PopulationContext_ABC
{
public:
    DEC_PopulationContext( const MIL_Population& population, const T_PointVector& points );

private:
    virtual const T_PopulationPathChannelers& GetChannelers() const;
    virtual double GetCostOutsideOfChanneling() const;
    virtual double GetCostOutsideOfAllObjects() const;
    virtual const T_PathKnowledgeObjectsByTypes& GetPathKnowledgeObjects() const;
    virtual double GetMaxSpeed() const;
    virtual double GetMaxSpeed( const TerrainData& terrainData ) const;

private:
    void InitializePathKnowledges( const MIL_Population& population, const T_PointVector& pathPoints );

private:
    const DEC_Population_PathClass& pathClass_;
    const MIL_PopulationType& type_;
    T_PopulationPathChannelers channelers_;
    T_PathKnowledgeObjectsByTypes pathKnowledgeObjects_;
    double rCostOutsideOfAllObjects_;
};

#endif // __DEC_PopulationContext_h_
