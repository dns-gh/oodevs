//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_Path.h $
// $Author: Age $
// $Modtime: 16/06/05 15:09 $
// $Revision: 10 $
// $Workfile: DEC_Agent_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Agent_Path_h_
#define __DEC_Agent_Path_h_

#include "MIL.h"

#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"
#include "Decision/Path/DEC_PathResult.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Network/NET_ASN_Types.h"
#include "MT_Tools/MT_Profiler.h"

class DEC_Rep_PathPoint;
class DEC_PathType;
class MIL_Lima;
class MIL_AgentPion;

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Agent_Path : public DEC_PathResult
{
public:
    //! @name Types
    //@{   
    typedef std::vector< DEC_Path_KnowledgeAgent >     T_PathKnowledgeAgentVector;
    typedef T_PathKnowledgeAgentVector::const_iterator CIT_PathKnowledgeAgentVector;

    typedef std::vector< DEC_Path_KnowledgeObject >     T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator CIT_PathKnowledgeObjectVector;
    //@}

public:
     DEC_Agent_Path( const MIL_AgentPion& queryMaker, const MT_Vector2D&   vPosEnd, const DEC_PathType& pathType );
     DEC_Agent_Path( const MIL_AgentPion& queryMaker, const T_PointVector& points , const DEC_PathType& pathType );
     DEC_Agent_Path( const DEC_Agent_Path& rhs ); // Copy only query parameters, not the result !
    ~DEC_Agent_Path();

    //! @name Path calculation
    //@{
    virtual void Execute              ( TerrainPathfinder& pathfind );
    virtual void CleanAfterComputation();
    virtual bool NeedRefine           () const;
    //@}
    
    //! @name Accessors 
    //@{
    const MIL_Fuseau&                   GetFuseau              () const;
    const MIL_Fuseau&                   GetAutomataFuseau      () const;
    const PHY_Speeds&                   GetUnitSpeeds          () const;
          MT_Float                      GetUnitMaxSlope        () const;
    const MT_Vector2D&                  GetDirDanger           () const;
    const T_PathKnowledgeObjectVector&  GetPathKnowledgeObjects() const;
    const T_PathKnowledgeAgentVector&   GetPathKnowledgeAgents () const;
    const DEC_PathType&                 GetPathType            () const;
    const DEC_PathClass&                GetPathClass           () const;         
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

    void InsertPointAvant        ( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent );
    void InsertPointAvant        ( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPointAvant );    
    bool InsertPoint             ( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint );
    void InsertPointAndPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint, MT_Float& rDistSinceLastPointAvant );

    void InsertLima       ( const MIL_Lima& );

    IT_PathPointList GetPreviousPathPointOnDifferentLocation( IT_PathPointList );
    //@}
   
private:   
    const MIL_AgentPion& queryMaker_;   
    
    // Path calculation parameters
    const DEC_PathType&                pathType_;
    const DEC_PathClass&               pathClass_;
          bool                         bRefine_;
          T_PointVector                pathPoints_;
          MIL_Fuseau                   fuseau_;
          MIL_Fuseau                   automateFuseau_;
          MT_Vector2D                  vDirDanger_;
          PHY_Speeds                   unitSpeeds_;
          MT_Float                     rMaxSlope_;
          T_PathKnowledgeAgentVector   pathKnowledgeAgentVector_;
          T_PathKnowledgeObjectVector  pathKnowledgeObjectVector_;
          MT_Profiler                  profiler_;
    
    //! @name
    //@{
    bool bDecPointsInserted_;
    //@}
};

#include "DEC_Agent_Path.inl"

#endif // __DEC_Agent_Path_h_