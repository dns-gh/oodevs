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

#include "TER_World_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>

class TerrainData;
class MT_Ellipse;
class TER_Agent_ABC;
class TER_Object_ABC;
class TER_Localisation;
class TER_Polygon;
class TER_AgentPositionHint;
class TER_ObjectPositionHint;
class TER_AgentManager;
class TER_ObjectManager;
class TER_PopulationManager;
class TER_CoordinateManager;
class TER_StaticData;
class TER_PathFindManager;
class TER_Analyzer;
class TER_LimitDataManager;

namespace tools
{
namespace thread
{
    template< typename T > class MessageQueue_ABC;
}

    class ExerciseConfig;
}

// =============================================================================
// Created: AGE 2005-01-28
// =============================================================================
class TER_World : public TER_World_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< TER_Agent_ABC* >  T_AgentVector;
    typedef std::vector< TER_Object_ABC* > T_ObjectVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit TER_World( const tools::ExerciseConfig& config );
            ~TER_World();
    //@}

    //! @name Operations
    //@{
    static TER_World& GetWorld();
    static const boost::shared_ptr< TER_World >& GetWorldPtr();
    static void Initialize( const tools::ExerciseConfig& config );
    static void DestroyWorld();
    //@}

    //! @name Coordinates
    //@{
    virtual void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const;
    virtual void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const;

    virtual void MosToSimMgrsCoord( double latitude, double longitude, MT_Vector2D& pos ) const;
    virtual void SimToMosMgrsCoord( const MT_Vector2D& pos, double& latitude, double& longitude ) const;

    virtual double GetWeldValue() const;
    virtual double GetWidth    () const;
    virtual double GetHeight   () const;
    virtual const MT_Rect& GetExtent() const;

    virtual bool IsValidPosition( const MT_Vector2D& pos ) const;
    virtual void ClipPointInsideWorld( MT_Vector2D& pos ) const;
    virtual MT_Vector2D ClipPointInsideWorld( const MT_Vector2D& pos ) const;
    //@}

    //! @name Accessors
    //@{
    TER_PathFindManager&   GetPathFindManager  () const;
    TER_Analyzer&          GetAnalyzer() const;
    TER_AgentManager&      GetAgentManager     () const;
    TER_ObjectManager&     GetObjectManager    () const;
    TER_PopulationManager& GetPopulationManager() const;
    TER_LimitDataManager& GetLimitManager() const;
    //@}

private:
    //! @name Member data
    //@{
    TER_AgentManager*      pAgentManager_;
    TER_ObjectManager*     pObjectManager_;
    TER_CoordinateManager* pCoordinateManager_;
    TER_StaticData*        pGraphManager_;
    TER_PathFindManager*   pPathfindManager_;
    TER_PopulationManager* pPopulationManager_;
    std::unique_ptr< TER_Analyzer > analyzer_;
    std::unique_ptr< TER_LimitDataManager > limitManager_;
    //@}
};

// Initialize TER_World singleton, mostly for tests.
class FakeWorld
{
public:
    explicit FakeWorld( const std::string& exercise );
    virtual ~FakeWorld();
};

boost::shared_ptr< TER_World > CreateWorld( const std::string& exercise );

#endif // __TER_World_h_
