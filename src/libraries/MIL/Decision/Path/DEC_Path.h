//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.h $
// $Author: Age $
// $Modtime: 16/06/05 15:09 $
// $Revision: 10 $
// $Workfile: DEC_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Path_h_
#define __DEC_Path_h_

#include "MIL.h"

#include "DEC_Path_ABC.h"

#include "DEC_Path_KnowledgeAgent.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_PathType.h"

#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Network/NET_ASN_Types.h"
#include "Tools/MT_Profiler.h"

class DEC_Rep_PathPoint;
class DEC_PathPoint;
class MIL_Lima;
class MIL_AgentPion;

// $$$$ NLD 2004-01-28 - Cette classe est une $$JVT 2004-11-09 grosse$$ merde

//*****************************************************************************
// Created: JDY 03-02-11
// Last modified: JVT 03-11-26
//*****************************************************************************
class DEC_Path : public DEC_Path_ABC
{
    friend class DEC_PathFind_ComputationThread;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{   
    typedef std::list< DEC_PathPoint* >                 T_PathPointList;
    typedef T_PathPointList::iterator                   IT_PathPointList;
    typedef T_PathPointList::const_iterator             CIT_PathPointList;    

    typedef std::multimap< MT_Float, DEC_Knowledge_Object* >  T_KnowledgeObjectMultimap;
    typedef T_KnowledgeObjectMultimap::iterator               IT_KnowledgeObjectMultimap;
    typedef T_KnowledgeObjectMultimap::const_iterator         CIT_KnowledgeObjectMultimap;

    typedef std::vector< DEC_Path_KnowledgeAgent >     T_PathKnowledgeAgentVector;
    typedef T_PathKnowledgeAgentVector::const_iterator CIT_PathKnowledgeAgentVector;

    typedef std::vector< DEC_Path_KnowledgeObject >     T_PathKnowledgeObjectVector;
    typedef T_PathKnowledgeObjectVector::const_iterator CIT_PathKnowledgeObjectVector;
    //@}

public:
     DEC_Path( MIL_AgentPion& queryMaker, const MT_Vector2D&   vPosEnd, const DEC_PathType& pathType );
     DEC_Path( MIL_AgentPion& queryMaker, const T_PointVector& points , const DEC_PathType& pathType );
     DEC_Path( const DEC_Path& rhs ); // Copy only query parameters, not the result !
    ~DEC_Path();

    //! @name Path calculation
    //@{
    virtual void Execute( TerrainPathfinder& pathfind );
            void AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    //@}
    
    //! @name Operations
    //@{
    bool operator==( const DEC_Path& rhs ) const;
    bool operator!=( const DEC_Path& rhs ) const;
    //@}

    //! @name Accessors 
    //@{
          uint                          GetID                  () const;    
    const MIL_AgentPion&                GetQueryMaker          () const;
    const T_PathPointList&              GetResult              () const;
    const MIL_Fuseau&                   GetFuseau              () const;
    const MIL_Fuseau&                   GetAutomataFuseau      () const;
    const PHY_Speeds&                   GetUnitSpeeds          () const;
    MT_Float                            GetUnitMaxSlope        () const;
    const MT_Vector2D&                  GetDirDanger           () const;
    const T_PathKnowledgeObjectVector&  GetPathKnowledgeObjects() const;
    const T_PathKnowledgeAgentVector&   GetPathKnowledgeAgents () const;
    const DEC_PathType&                 GetPathType            () const;
    //@}
    
    //! @name Tools
    //@{
    CIT_PathPointList GetCurrentKeyOnPath  ( const MT_Vector2D& vPos ) const;
    MT_Vector2D       GetPointOnPathCloseTo( const MT_Vector2D& posToTest ) const;
    void              InsertDecPoints      ();

    bool              CanQueryMakerFly() const;
    //@}

    //! @name Operations
    //@{
    MT_Vector2D GetFuturePosition            ( const MT_Vector2D& vStartPos, MT_Float rDist, bool bBoundOnPath ) const;
    void        ComputeFutureObjectCollisions( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, T_KnowledgeObjectMultimap& objectsOnPathMap ) const;
    //@}

    //! @name Network
    //@{
    void Serialize( ASN1T_Itineraire& asn ) const;
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

    void InsertPointAvant ( IT_PathPointList itCurrent, DEC_Rep_PathPoint& spottedPathPoint );
    void InsertLima       ( const MIL_Lima& );

    MT_Vector2D InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, MT_Float rDist, bool bBoundOnPath ) const;
    IT_PathPointList GetPreviousPathPointOnDifferentLocation( IT_PathPointList );
    //@}

    //! @name Operators
    //@{
    DEC_Path& operator=( const DEC_Path& rhs );
    //@}
    
private:   
    uint           nID_;
    MIL_AgentPion& queryMaker_;   
    
    // Path calculation parameters
    DEC_PathType                 pathType_;
    MIL_Fuseau                   fuseau_;
    MT_Vector2D                  vDirDanger_;
    PHY_Speeds                   unitSpeeds_;
    MT_Float                     rMaxSlope_;
    T_PathKnowledgeAgentVector   pathKnowledgeAgentVector_;
    T_PathKnowledgeObjectVector  pathKnowledgeObjectVector_;

    MT_Profiler                  profiler_;
    //@}   
    
    //! @name
    //@{
    bool            bDecPointsInserted_;
    T_PathPointList resultList_;
    //@}

private:
    static uint nIDIdx_;    

public:
    static const uint nInvalidID_;   
};

#include "DEC_Path.inl"

#endif // __DEC_Path_h_