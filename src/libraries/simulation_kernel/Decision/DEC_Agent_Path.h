// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Agent_Path_h_
#define __DEC_Agent_Path_h_

#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "Decision/DEC_PathResult.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/shared_ptr.hpp>

class DEC_Rep_PathPoint;
class DEC_PathType;
class MIL_LimaOrder;
class MIL_AgentPion;
class DEC_Agent_PathClass;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Agent_Path : public DEC_PathResult
{
public:
    //! @name Types
    //@{
    typedef std::vector< DEC_Path_KnowledgeAgent >       T_PathKnowledgeAgentVector;
    typedef T_PathKnowledgeAgentVector::const_iterator CIT_PathKnowledgeAgentVector;

    typedef std::vector< DEC_Path_KnowledgeObject >       T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator CIT_PathKnowledgeObjectVector;

    typedef std::vector< T_PathKnowledgeObjectVector >           T_PathKnowledgeObjectByTypesVector;
    typedef T_PathKnowledgeObjectByTypesVector::const_iterator CIT_PathKnowledgeObjectByTypesVector;

    typedef std::vector< DEC_Path_KnowledgePopulation >       T_PathKnowledgePopulationVector;
    typedef T_PathKnowledgePopulationVector::const_iterator CIT_PathKnowledgePopulationVector;
    //@}

public:
     DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const MT_Vector2D&   vPosEnd, const DEC_PathType& pathType );
     DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const MT_Vector2D&   vPosEnd, const DEC_PathType& pathType, bool loaded  );
     DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, std::vector< boost::shared_ptr< MT_Vector2D > >& points , const DEC_PathType& pathType );
     DEC_Agent_Path( const MIL_Agent_ABC& queryMaker, const T_PointVector& points , const DEC_PathType& pathType );
     DEC_Agent_Path( const DEC_Agent_Path& rhs ); // Copy only query parameters, not the result !
    virtual ~DEC_Agent_Path();

    //! @name Path calculation
    //@{
    virtual void Execute              ( TerrainPathfinder& pathfind );
    virtual void CleanAfterComputation();
    virtual bool NeedRefine           () const;
    virtual bool UseStrictClosest     () const;
    //@}

    //! @name Accessors
    //@{
    const MIL_Fuseau&                         GetFuseau                   () const;
    const MIL_Fuseau&                         GetAutomataFuseau           () const;
    const PHY_Speeds&                         GetUnitSpeeds               () const;
          MT_Float                            GetUnitMaxSlope             () const;
    const MT_Vector2D&                        GetDirDanger                () const;
    const T_PathKnowledgeObjectByTypesVector& GetPathKnowledgeObjects     () const;
          MT_Float                            GetCostOutsideOfAllObjects  () const;
    const T_PathKnowledgeAgentVector&         GetPathKnowledgeAgents      () const;
    const T_PathKnowledgePopulationVector&    GetPathKnowledgePopulations () const;
    const DEC_PathType&                       GetPathType                 () const;
    const DEC_Agent_PathClass&                GetPathClass                () const;
          MT_Float                            GetUnitMajorWeight          () const;
    //@}

    //! @name Tools
    //@{
    virtual void InsertDecPoints();
    //@}

private:
    //! @name Init
    //@{
    void Initialize              ( const T_PointVector& pathPoints );
    void InitializePathKnowledges( const T_PointVector& pathPoints );
    //@}

    //! @name Points insertion Tools
    //@{
    int  IsPointAvant     ( DEC_PathPoint* pBefore, DEC_PathPoint& current, DEC_PathPoint* pAfter, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvant     ( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantOut  ( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;
    bool IsPointAvantIn   ( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const;

    void InsertPointAvants();
    void InsertLimas      ();

    void InsertPointAvant        ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent );
    void InsertPointAvant        ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPointAvant );
    bool InsertPoint             ( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint );
    void InsertPointAndPointAvant( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint, MT_Float& rDistSinceLastPointAvant );
    void InsertLima              ( const MIL_LimaOrder& lima );

    IT_PathPointList GetPreviousPathPointOnDifferentLocation( IT_PathPointList );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& queryMaker_;
    // Path calculation parameters
    const DEC_PathType&                      pathType_; //$$$ A VIRER
    const DEC_Agent_PathClass&               pathClass_;
          bool                               bRefine_;
          T_PointVector                      pathPoints_;
          MIL_Fuseau                         fuseau_;
          MIL_Fuseau                         automateFuseau_;
          MT_Vector2D                        vDirDanger_;
          PHY_Speeds                         unitSpeeds_;
          MT_Float                           rMaxSlope_;
          T_PathKnowledgeAgentVector         pathKnowledgeAgents_;
          T_PathKnowledgeObjectByTypesVector pathKnowledgeObjects_;
          MT_Float                           rCostOutsideOfAllObjects_;
          T_PathKnowledgePopulationVector    pathKnowledgePopulations_;
          MT_Profiler                        profiler_;
          bool bDecPointsInserted_;
    //@}
};

#include "DEC_Agent_Path.inl"

#endif // __DEC_Agent_Path_h_
