// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef movement_module_PathWalker_h_
#define movement_module_PathWalker_h_

#include "PathResult.h"
#include "MT_Tools/MT_Vector2D.h"
#include <spatialcontainer/TerrainData.h>
#include <tools/Set.h>
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>

class KnowledgeCache;
class MT_Line;
struct SWORD_Model;

namespace sword
{
    class Path;

namespace wrapper
{
    class View;
}

namespace movement
{
    class ModuleFacade;

// =============================================================================
// Created: NLD 2005-09-30
// =============================================================================
class PathWalker : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,
        eFinished,
        ePaused,
        eNotEnoughFuel,
        eNotAllowed, // Neutralized
        eAlreadyMoving,
        eItineraireMustBeJoined,
        ePartialPath,
        eBlockedByObject,
        eTeleported
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PathWalker( ModuleFacade& module, unsigned int entity );
    virtual ~PathWalker();
    //@}

    //! @name Operations
    //@{
    E_ReturnCode Move( boost::shared_ptr< PathResult > path, const wrapper::View& model, const wrapper::View& entity ) const;
    void MoveSuspended( boost::shared_ptr< PathResult >, const wrapper::View& entity ) const;
    void MoveStopped( const wrapper::View& entity ) const;
    void MoveCanceled( boost::shared_ptr< PathResult > ) const;
    void Clean();
    //@}

    //! @name Tools
    //@{
    bool ComputeFutureObjectCollision( const wrapper::View& model, const wrapper::View& entity, const KnowledgeCache* objectsToTest,
                                       double& rDistance, wrapper::View* knowledgeObjectColliding, bool blockedByObject, bool applyScale ) const;
    MT_Vector2D ExtrapolatePosition( const wrapper::View& entity, const double rTime, const bool bBoundOnPath ) const;
    bool IsMovingOn( boost::shared_ptr< Path_ABC > path ) const;
    //@}

private:
     //! @name Types
    //@{
    typedef tools::Set< const SWORD_Model* > T_ObjectSet;

    // Struct used to store the steps when moving from a point to another : manage the collision with the dynamic objects
    struct T_MoveStep
    {
    public:
        T_MoveStep() {}
        T_MoveStep( const MT_Vector2D& vPos ) : vPos_( vPos ) {}
    public:
        MT_Vector2D vPos_;
        T_ObjectSet objectsToNextPointSet_; // The path to the next point pass through these objects
        T_ObjectSet objectsOutSet_;         // On this move step, we are out of these objets
        T_ObjectSet ponctualObjectsOnSet_;  // Ponctual objets that are on this move step point (i.e. Objects that aren't on the path to the next point)
    };

    // STL comparison operator : use the SquareDistance between the start pos and the collision pos
    struct sMoveStepCmp : std::binary_function< const T_MoveStep& , const T_MoveStep&, bool >
    {
    public:
        sMoveStepCmp( const MT_Vector2D& vPosStart )
            : vPosStart_( vPosStart )
        {};

        bool operator() ( const T_MoveStep& vPos1, const T_MoveStep& vPos2 ) const
        {
            return vPosStart_.SquareDistance( vPos1.vPos_ ) < vPosStart_.SquareDistance( vPos2.vPos_ );
        };
    private:
        MT_Vector2D vPosStart_;
    };

    typedef std::set< T_MoveStep, sMoveStepCmp > T_MoveStepSet;
    typedef T_MoveStepSet::iterator             IT_MoveStepSet;
    typedef T_MoveStepSet::const_iterator      CIT_MoveStepSet;
    //@}

private:
    //! @name Tools
    //@{
    bool TryToMoveToNextStep( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, double& rTimeRemaining, bool bFirstMove, const wrapper::View& entity ) const;
    bool TryToMoveTo( boost::shared_ptr< PathResult > path, const MT_Vector2D& vNewPosTmp, double& rTimeRemaining, const wrapper::View& model, const wrapper::View& entity ) const;
    void ComputeObjectsCollision( const wrapper::View& model, const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet ) const;
    void ComputeCurrentSpeed( const wrapper::View& entity ) const;
    void InitializeEnvironment( boost::shared_ptr< PathResult > path, const wrapper::View& entity ) const;
    bool GoToNextNavPoint( boost::shared_ptr< PathResult > path, const wrapper::View& entity ) const;
    E_ReturnCode SetCurrentPath( boost::shared_ptr< PathResult > path, const wrapper::View& model, const wrapper::View& entity ) const;
    void SetCurrentPathPoint( boost::shared_ptr< PathResult > path ) const;
    void CheckPathNotification( const wrapper::View& entity ) const;
    void SetEnvironmentType( const TerrainData& environment, const wrapper::View& entity ) const;
    void PostMovement( const wrapper::View& entity ) const;
    void PostPath( const wrapper::View& entity ) const;
    void PostEnvironment( const wrapper::View& entity, const TerrainData& environment ) const;

    struct CollisionData
    {
        const MT_Line* lineTmp;
        T_PointSet* collisions;
        T_MoveStepSet* moveStepSet;
    };
    static void ComputeObjectCollision( const SWORD_Model* object, void* userData );
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    unsigned int entity_;
    mutable TerrainData environment_;
    mutable PathResult::CIT_PathPointList itNextPathPoint_;
    mutable PathResult::CIT_PathPointList itCurrentPathPoint_;
    // Effect
    mutable MT_Vector2D vNewPos_;
    mutable MT_Vector2D vNewDir_;
    mutable double speed_;
    mutable double distance_;
    mutable std::size_t pointsPassed_;
    mutable bool bForcePathCheck_;
    mutable bool bHasMoved_;
    mutable bool bFuelReportSent_;
    mutable boost::weak_ptr< PathResult > path_;
    mutable E_ReturnCode pathSet_;
    //@}
};

}
}

#endif // movement_module_PathWalker_h_
