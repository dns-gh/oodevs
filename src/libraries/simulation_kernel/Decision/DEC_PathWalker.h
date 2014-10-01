// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_PathWalker_h_
#define __DEC_PathWalker_h_

#include "DEC_PathResult.h"
#include "Entities/Effects/MIL_Effect_Move.h"
#include "MT_Tools/MT_Vector2D.h"
#include <spatialcontainer/TerrainData.h>
#include <tools/Set.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <set>

namespace sword
{
    class Path;
    class UnitEnvironmentType;
}

class MIL_Agent_ABC;
class MIL_Object_ABC;
class PHY_MovingEntity_ABC;

// =============================================================================
// Created: NLD 2005-09-30
// =============================================================================
class DEC_PathWalker
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
    explicit DEC_PathWalker( PHY_MovingEntity_ABC& movingEntity );
    virtual ~DEC_PathWalker();
    //@}

    //! @name Operations
    //@{
    int Move( const boost::shared_ptr< DEC_PathResult >& pPath );
    void MoveSuspended( const boost::shared_ptr< DEC_PathResult >& pPath );
    void MoveCanceled( const boost::shared_ptr< DEC_PathResult >& pPath );
    void Apply(); // Called by MIL_Effect_Move
    void Clean();
    bool HasCurrentPath() const;
    //@}

    //! @name Tools
    //@{
    bool ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest,
        double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject,
        const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const;
    MT_Vector2D ExtrapolatePosition( const MT_Vector2D& position, const double rSpeed, const double rTime, const bool bBoundOnPath ) const;
    bool IsMovingOn( const DEC_Path_ABC& path ) const;
    //@}

    //! @name Network
    //@{
    void SerializeEnvironmentType( sword::UnitEnvironmentType& msg ) const;
    void SerializeCurrentPath( sword::Path& asn ) const;
    //@}

private:
     //! @name Types
    //@{
    typedef tools::Set< MIL_Object_ABC* > T_ObjectSet;

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
        {}
        bool operator() ( const T_MoveStep& vPos1, const T_MoveStep& vPos2 ) const
        {
            return vPosStart_.SquareDistance( vPos1.vPos_ ) < vPosStart_.SquareDistance( vPos2.vPos_ );
        }
    private:
        MT_Vector2D vPosStart_;
    };

    typedef std::set< T_MoveStep, sMoveStepCmp > T_MoveStepSet;
    typedef T_MoveStepSet::iterator             IT_MoveStepSet;
    typedef T_MoveStepSet::const_iterator      CIT_MoveStepSet;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_PathWalker( const DEC_PathWalker& );            //!< Copy constructor
    DEC_PathWalker& operator=( const DEC_PathWalker& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    bool TryToMoveToNextStep( const MT_Vector2D& startPosition, CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, double& rTimeRemaining );
    bool TryToMoveTo( const MT_Vector2D& vNewPosTmp, double& rTimeRemaining );
    void ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet );
    void ComputeCurrentSpeed();
    void InitializeEnvironment( const DEC_PathResult& path );
    bool GoToNextNavPoint( DEC_PathResult& path );
    E_ReturnCode SetCurrentPath( boost::shared_ptr< DEC_PathResult > pPath );
    void SetCurrentPathPoint( DEC_PathResult& path );
    void CheckPathNotification();
    bool HandleObject( const MT_Vector2D& startPosition, const MT_Vector2D& endPosition, MIL_Object_ABC& object, double& rMaxSpeedForStep, bool ponctual );
    MT_Vector2D ComputePositionBeforeObject( const MT_Vector2D& startPosition, const MT_Vector2D& currentStepPos, const MIL_Object_ABC& object ) const;
    boost::shared_ptr< DEC_Knowledge_Object > FindBlockingObject( const MT_Vector2D& endPosition, const MIL_Object_ABC& object ) const;
     //@}

private:
    //! @name Member data
    //@{
    PHY_MovingEntity_ABC& movingEntity_;
    TerrainData environment_;
    DEC_PathResult::T_PathPoints::const_iterator itNextPathPoint_;
    DEC_PathResult::T_PathPoints::const_iterator itCurrentPathPoint_;
    // Effect
    MIL_Effect_Move effectMove_;
    MT_Vector2D vNewPos_;
    MT_Vector2D vNewDir_;
    double rCurrentSpeed_;
    double rWalkedDistance_;
    int pointsPassed_;
    bool bForcePathCheck_;
    bool bHasMoved_;
    bool bFuelReportSent_;
    bool bImpossibleReportSent_;
    boost::shared_ptr< DEC_PathResult > pCurrentPath_;
    boost::weak_ptr< DEC_Knowledge_Object > collision_;
    E_ReturnCode pathSet_;
    //@}
};

#endif // __DEC_PathWalker_h_
