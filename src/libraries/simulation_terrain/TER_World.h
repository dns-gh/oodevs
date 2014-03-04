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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

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
class TER_GraphManager;
class TER_PathFindManager;
class TER_AnalyzerManager;
class TER_LimitDataManager;
class MT_Rect;
class MT_Vector2D;

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
class TER_World : boost::noncopyable
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
    void MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const;
    void SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const;

    void MosToSimMgrsCoord( double latitude, double longitude, MT_Vector2D& pos ) const;
    void SimToMosMgrsCoord( const MT_Vector2D& pos, double& latitude, double& longitude ) const;

    double GetWeldValue() const;
    double GetWidth    () const;
    double GetHeight   () const;
    const MT_Rect& GetExtent() const;

    bool IsValidPosition( const MT_Vector2D& pos ) const;
    void ClipPointInsideWorld( MT_Vector2D& pos ) const;
    MT_Vector2D ClipPointInsideWorld( const MT_Vector2D& pos ) const;
    //@}

    //! @name Accessors
    //@{
    TER_PathFindManager&   GetPathFindManager  () const;
    TER_AnalyzerManager&   GetAnalyzerManager  () const;
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
    TER_GraphManager*      pGraphManager_;
    TER_PathFindManager*   pPathfindManager_;
    TER_AnalyzerManager*   pAnalyzerManager_;
    TER_PopulationManager* pPopulationManager_;
    std::unique_ptr< TER_LimitDataManager > limitManager_;
    //@}
};

#endif // __TER_World_h_
