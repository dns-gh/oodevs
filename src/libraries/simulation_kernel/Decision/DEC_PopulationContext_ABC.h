// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Agent_PathfindContext_ABC_h_
#define __DEC_Agent_PathfindContext_ABC_h_

#include "DEC_Population_Path_Channeler.h"
#include <boost/noncopyable.hpp>
#include <vector>

class DEC_Path_KnowledgeObject_ABC;

// =============================================================================
/** @class  DEC_PopulationContext_ABC
    @brief  DEC_PopulationContext_ABC
*/
// Created: MCO 2014-05-16
// =============================================================================
class DEC_PopulationContext_ABC : boost::noncopyable
{
protected:
    typedef std::vector< DEC_Population_Path_Channeler > T_PopulationPathChannelers;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > > T_PathKnowledgeObjects;
    typedef std::vector< T_PathKnowledgeObjects > T_PathKnowledgeObjectsByTypes;

public:
             DEC_PopulationContext_ABC() {}
    virtual ~DEC_PopulationContext_ABC() {}

    virtual const T_PopulationPathChannelers& GetChannelers() const = 0;
    virtual double GetCostOutsideOfChanneling() const = 0;
    virtual double GetCostOutsideOfAllObjects() const = 0;
    virtual const T_PathKnowledgeObjectsByTypes& GetPathKnowledgeObjects() const = 0;
    virtual double GetMaxSpeed() const = 0;
    virtual double GetMaxSpeed( const TerrainData& terrainData ) const = 0;
};

#endif // __DEC_Agent_PathfindContext_ABC_h_
