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
    class Model;
}

namespace sword
{
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
    //! @name Constructors/Destructor
    //@{
    virtual ~PathAdapter();
    //@}

    //! @name Operations
    //@{
    static std::size_t Add();
    static const boost::shared_ptr< PathAdapter >& Get( std::size_t path );
    static boost::shared_ptr< PathAdapter > Remove( std::size_t path );

    virtual void Execute( TER_Pathfinder_ABC& pathfind );
    virtual void CleanAfterComputation();

    virtual double GetLength() const;
    virtual E_State GetState() const;

    double GetAltitudeCost( const MT_Vector2D& from, const MT_Vector2D& to, double costPerMeter ) const;
    double GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to,
                           double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, // $$$$ MCO : all those configuration values should stay out of the movement module
                           double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
                           double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) const;
    double GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    double GetUrbanBlockCost( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    double GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) const;
    double GetPopulationsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) const;

    void Initialize( const core::Model& entity );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PathAdapter();
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
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgeObject_ABC > >  T_PathKnowledgeObjectVector;
    typedef std::vector< T_PathKnowledgeObjectVector > T_PathKnowledgeObjectByTypesVector;
    typedef std::vector< boost::shared_ptr< DEC_Path_KnowledgePopulation > > T_PathKnowledgePopulationVector;
    //@}

private:
    //! @name Member data
    //@{
    size_t entity_;
    const PHY_RawVisionData& data_;
    MIL_Fuseau fuseau_;
    MIL_Fuseau automateFuseau_;
    T_PathKnowledgeAgentVector pathKnowledgeAgents_;
    T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
    T_PathKnowledgePopulationVector pathKnowledgePopulations_;
    double weight_, squareSlope_;
    E_CrossingHeight height_;
    double rCostOutsideOfAllObjects_;
    //@}
};

}

#endif // SWORD_PATH_ADAPTER_H
