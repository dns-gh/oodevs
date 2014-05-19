// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_AgentContext_ABC_h_
#define __DEC_AgentContext_ABC_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class MIL_Fuseau;
class PHY_Speeds;
class MT_Vector2D;
class DEC_Agent_PathClass;
class DEC_Path_KnowledgeAgent;
class DEC_Path_KnowledgeObject_ABC;
class DEC_Path_KnowledgePopulation;

// =============================================================================
/** @class  DEC_AgentContext_ABC
    @brief  DEC_AgentContext_ABC
*/
// Created: MCO 2014-05-16
// =============================================================================
class DEC_AgentContext_ABC : boost::noncopyable
{
protected:
    typedef std::vector< DEC_Path_KnowledgeAgent > T_PathKnowledgeAgentVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > > T_PathKnowledgeObjectVector;
    typedef std::vector< T_PathKnowledgeObjectVector > T_PathKnowledgeObjectByTypesVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgePopulation > > T_PathKnowledgePopulationVector;

public:
             DEC_AgentContext_ABC() {}
    virtual ~DEC_AgentContext_ABC() {}

    virtual const MIL_Fuseau& GetFuseau() const = 0;
    virtual const MIL_Fuseau& GetAutomataFuseau() const = 0;
    virtual const PHY_Speeds& GetUnitSpeeds() const = 0;
    virtual const MT_Vector2D& GetDirDanger() const = 0;

    virtual const DEC_Agent_PathClass& GetPathClass() const = 0;

    virtual double GetUnitMaxSlope() const = 0;
    virtual double GetUnitSlopeDeceleration() const = 0;
    virtual double GetUnitMajorWeight() const = 0;
    virtual double GetCostOutsideOfAllObjects() const = 0;

    virtual const T_PathKnowledgeAgentVector& GetPathKnowledgeAgents() const = 0;
    virtual const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects() const = 0;
    virtual const T_PathKnowledgePopulationVector& GetPathKnowledgePopulations() const = 0;
};

#endif // __DEC_AgentContext_ABC_h_
