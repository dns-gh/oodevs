// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_World.h $
// $Author: Nld $
// $Modtime: 11/04/05 15:27 $
// $Revision: 5 $
// $Workfile: TER_World.h $
//
// *****************************************************************************

#ifndef __TER_World_h_
#define __TER_World_h_

#include "TER.h"
#include "TER_NodeFunctor_ABC.h"
#include "TER_CoordinateManager.h"

class TerrainData;
class MT_Ellipse;
class TER_Agent_ABC;
class TER_DynaObject_ABC;
class TER_Localisation;
class TER_Polygon;
class TER_AgentPositionHint;
class TER_ObjectPositionHint;
class TER_AgentManager;
class TER_ObjectManager;
class TER_CoordinateManager;
class TER_PathFindManager;
template< typename T > class MT_InputArchive_Logger;
class MT_XXmlInputArchive;
class TER_PathFinderThread;
class MT_Rect;

class TER_PathFindRequest_ABC;
namespace tools
{
    namespace thread
    {
        template< typename T > class MessageQueue_ABC;
    }
}

// =============================================================================
/** @class  TER_World
    @brief  TER_World
*/
// Created: AGE 2005-01-28
// =============================================================================
class TER_World
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Agent_ABC* >      T_AgentVector;
    typedef std::vector< TER_DynaObject_ABC* > T_DynaObjectVector;
    //@}

public:
    //! @name Operations
    //@{
    static TER_World& GetWorld();
    static void Initialize( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive );
    static void DestroyWorld();
    //@}

    //! @name Coordinates
    //@{
    void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const;
    void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const;

    MT_Float GetMeterPerPixel() const; // $$$$ AGE 2005-01-31: 
    MT_Float GetMeterSquarePerPixelSquare() const;
    MT_Float GetWeldValue() const;
    MT_Float GetWidth    () const;
    MT_Float GetHeight   () const;

    bool IsValidPosition( const MT_Vector2D& pos ) const;
    void ClipPointInsideWorld( MT_Vector2D& pos ) const;
    MT_Vector2D ClipPointInsideWorld( const MT_Vector2D& pos ) const;
    //@}

    //! @name Pathfinder
    //@{
    TER_PathFinderThread& CreatePathFinderThread( tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue ) const;
    TerrainData GetTerrainDataAt( const MT_Vector2D& pos ) const; // Only used to dump on the console

    void ApplyFunctorOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;
    template < typename Functor >
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, Functor& bestNodeFunction ) const;
    //@}

    //! @name Agents
    //@{
    void GetListAgentWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agentVector ) const;
    void GetListAgentWithinCircle ( const MT_Vector2D& vCenter, MT_Float rRadius, T_AgentVector& agentVector ) const;
    void GetListAgentWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agentVector ) const;
    void GetListAgentWithinPolygon( const TER_Polygon& polygon, T_AgentVector& agentVector ) const;

    TER_AgentPositionHint UpdateAgentPosition( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint );
    bool RemoveAgent( TER_Agent_ABC& agent, const TER_AgentPositionHint& hint );
    //@}

    //! @name Objects
    //@{
    void GetListDynaObjectsAt( const MT_Vector2D& vPos, T_DynaObjectVector& dynaObjectsSet ) const;
    void GetListDynaObjectWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_DynaObjectVector& dynaObjectsSet ) const;        

    TER_ObjectPositionHint UpdateObjectPosition( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint );
    bool RemoveObject( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint );
    //@}

    //! @name Accessors
    //@{
    TER_PathFindManager& GetPathFindManager() const;
    
    const std::string& GetGraphFileName() const;
    const std::string& GetNodeFileName () const;
    const std::string& GetLinkFileName () const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit TER_World( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive );
             TER_World( const TER_World& );            //!< Copy constructor
    virtual ~TER_World();
    TER_World& operator=( const TER_World& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadWorld( const std::string& strWorld, float& rLatitude, float& rLongitude, MT_Rect& extent ) const;
    //@}

private:
    //! @name Member data
    //@{
    static TER_World*  pInstance_;
    TER_AgentManager*      pAgentManager_;
    TER_ObjectManager*     pObjectManager_;
    TER_CoordinateManager* pCoordinateManager_;
    TER_PathFindManager*   pPathfindManager_;
    
    const std::string graphFileName_;
    const std::string nodeFileName_;
    const std::string linkFileName_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: TER_World::SearchForBestNodePositionWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
template < typename Functor >
void TER_World::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, Functor& bestNodeFunction ) const
{
    TER_NodeFunctor< Functor > functor( bestNodeFunction );
    ApplyFunctorOnNodesWithinCircle( vCenter, rRadius, functor );
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
TER_World& TER_World::GetWorld()
{
    assert( pInstance_ );
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetMeterPerPixel
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetMeterPerPixel() const
{
    return pCoordinateManager_->GetMeterPerPixel();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetMeterSquarePerPixelSquare
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetMeterSquarePerPixelSquare() const
{
    return pCoordinateManager_->GetMeterSquarePerPixelSquare();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetWeldValue
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetWeldValue() const
{
    return pCoordinateManager_->GetWeldValue();
}

#endif // __TER_World_h_
