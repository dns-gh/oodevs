// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PATH_ADAPTER_H
#define SWORD_PATH_ADAPTER_H

#include "Decision/DEC_Path_ABC.h"
#include "Decision/DEC_Agent_PathClass_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include <boost/shared_ptr.hpp>
#include <module_api/Model.h>

class PHY_RawVisionData;
class DEC_Path_KnowledgeAgent;
class DEC_Path_KnowledgePopulation;
class DEC_Path_KnowledgeObject_ABC;
class MIL_AgentPion;

namespace core
{
    class Facade;
    class Model;
}

namespace sword
{
namespace movement
{
    class Path_ABC;
}
// =============================================================================
/** @class  PathAdapter
    @brief  Path adapter

    The purpose of this adapter is twofold :
    . adapt sword::movement::Path_ABC to a DEC_Path_ABC to be used by the simulation
    . isolate data used to compute the cost for the pathfind because the code is executed in pathfind threads
*/
// Created: MCO 2012-01-26
// =============================================================================
class PathAdapter : public DEC_Path_ABC, private DEC_Agent_PathClass_ABC
{
public:
    //! @name Static Initializer
    //@{
    static void Initialize( core::Facade& facade );
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual ~PathAdapter();
    //@}

    //! @name Accessors
    //@{
    boost::shared_ptr< movement::Path_ABC > Get() const; // $$$$ MCO : to get rid of this we need to bind PathAdapter directly to DEC functions
    //@}

    //! @name Operations
    //@{
    static boost::shared_ptr< PathAdapter > Add( const core::Model& entity, const boost::shared_ptr< movement::Path_ABC >& path );
    static boost::shared_ptr< PathAdapter > Remove( const boost::shared_ptr< movement::Path_ABC >& path );

    virtual void Execute( TerrainPathfinder& pathfind );
    virtual void CleanAfterComputation();

    virtual double GetLength() const;
    virtual E_State GetState() const;

    void ComputePath() const;

    double GetAltitudeCost( const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) const;
    double GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to,
                           double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, // $$$$ MCO : all those configuration values should stay out of the movement module
                           double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
                           double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) const;
    double GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    double GetUrbanBlockCost( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    double GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) const;
    double GetPopulationsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PathAdapter( const core::Model& entity, const boost::shared_ptr< movement::Path_ABC >& path );
    //@}

    //! @name Helpers
    //@{
    void InitializePathKnowledges( const core::Model& entity, const MIL_AgentPion& pion );
    //@}

    //! @name Accessors
    //@{
    virtual double GetPopulationSecurityRange() const;
    virtual double GetCostOutsideOfPopulation() const;
    virtual double GetPopulationAttitudeCost( unsigned int attitudeID ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< DEC_Path_KnowledgeAgent >       T_PathKnowledgeAgentVector;
    typedef T_PathKnowledgeAgentVector::const_iterator CIT_PathKnowledgeAgentVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > >  T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator CIT_PathKnowledgeObjectVector;
    typedef std::vector< T_PathKnowledgeObjectVector > T_PathKnowledgeObjectByTypesVector;
    typedef T_PathKnowledgeObjectByTypesVector::const_iterator CIT_PathKnowledgeObjectByTypesVector;
    typedef std::vector< DEC_Path_KnowledgePopulation >       T_PathKnowledgePopulationVector;
    typedef T_PathKnowledgePopulationVector::const_iterator CIT_PathKnowledgePopulationVector;
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< movement::Path_ABC > path_;
    const PHY_RawVisionData& altitudeData_;
    MIL_Fuseau fuseau_;
    MIL_Fuseau automateFuseau_;
    T_PathKnowledgeAgentVector pathKnowledgeAgents_;
    T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
    T_PathKnowledgePopulationVector pathKnowledgePopulations_;
    double weight_, slope_;
    E_CrossingHeight height_;
    double rCostOutsideOfAllObjects_;
    //@}
};

}

#endif // SWORD_PATH_ADAPTER_H
