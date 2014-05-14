// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Agent_PathfinderPath_h_
#define __DEC_Agent_PathfinderPath_h_

#include "DEC_Path_KnowledgeAgent.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class MIL_Agent_ABC;
class PHY_Speeds;
class DEC_Agent_PathClass;
class DEC_Path_KnowledgeAgent;
class DEC_Path_KnowledgeObject_ABC;
class DEC_Path_KnowledgePopulation;

// =============================================================================
/** @class  DEC_Agent_PathfinderPath
    @brief  Provides path related context for the pathfinder
*/
// Created: MCO 2014-05-14
// =============================================================================
class DEC_Agent_PathfinderPath : boost::noncopyable
{
private:
    typedef std::vector< DEC_Path_KnowledgeAgent >T_PathKnowledgeAgentVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > > T_PathKnowledgeObjectVector;
    typedef std::vector< T_PathKnowledgeObjectVector > T_PathKnowledgeObjectByTypesVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgePopulation > > T_PathKnowledgePopulationVector;

public:
    DEC_Agent_PathfinderPath( const MIL_Agent_ABC& agent, const DEC_Agent_PathClass& pathClass, const T_PointVector& points );

    const MIL_Fuseau& GetFuseau() const;
    const MIL_Fuseau& GetAutomataFuseau() const;
    const PHY_Speeds& GetUnitSpeeds() const;
    const MT_Vector2D& GetDirDanger() const;

    const DEC_Agent_PathClass& GetPathClass() const;

    double GetUnitMaxSlope() const;
    double GetUnitSlopeDeceleration() const;
    double GetUnitMajorWeight() const;
    double GetCostOutsideOfAllObjects() const;

    const T_PathKnowledgeAgentVector& GetPathKnowledgeAgents() const;
    const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects() const;
    const T_PathKnowledgePopulationVector& GetPathKnowledgePopulations() const;

private:
    void Initialize( const MIL_Agent_ABC& agent, const T_PointVector& points );

private:
    const MIL_Fuseau fuseau_;
    const MIL_Fuseau automataFuseau_;
    const MT_Vector2D dangerDirection_;
    const double maxSlope_;
    const double slopeDeceleration_;
    const double majorWeight_;
    double costOutsideOfAllObjects_;
    const DEC_Agent_PathClass& class_;
    const PHY_Speeds speeds_;
    T_PathKnowledgeAgentVector pathKnowledgeAgents_;
    T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
    T_PathKnowledgePopulationVector pathKnowledgePopulations_;
};

#endif // __DEC_Agent_PathfinderPath_h_
