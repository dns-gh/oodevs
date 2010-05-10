// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_PathWalker_h_
#define __DEC_PathWalker_h_

#include "MIL.h"
#include "DEC_PathResult.h"
#include "Entities/Effects/MIL_Effect_Move.h"
#include <boost/shared_ptr.hpp>

namespace Common
{
    class MsgPath;
}

namespace MsgsSimToClient
{
    class MsgUnitEnvironmentType;
}

namespace urban
{
    class TerrainObject_ABC;
}

class MIL_Object_ABC;
class TerrainData;
class PHY_MovingEntity_ABC;

// ===================================/cygdrive/==========================================
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
        eItineraireMustBeJoined
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
    int  Move         ( boost::shared_ptr< DEC_PathResult > pPath );
    void MoveSuspended( boost::shared_ptr< DEC_PathResult > pPath );
    void MoveCanceled ( boost::shared_ptr< DEC_PathResult > pPath );

    void Apply(); // Called by MIL_Effect_Move

    void Clean();
    //@}

    //! @name Tools
    //@{
    bool        ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const;
    MT_Vector2D ExtrapolatePosition         ( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const;
    bool        IsMovingOn                  ( const DEC_Path_ABC& path ) const;
    //@}

    //! @name Network
    //@{
    void SerializeEnvironmentType( MsgsSimToClient::MsgUnitEnvironmentType& msg ) const;
    bool SerializeCurrentPath( Common::MsgPath& asn ) const;
    //@}

private:
     //! @name Types
    //@{
    typedef std::set< MIL_Object_ABC* > T_ObjectSet;
    typedef T_ObjectSet::const_iterator CIT_ObjectSet;
    typedef std::set< const urban::TerrainObject_ABC* >  T_UrbanBlockSet;
    typedef T_UrbanBlockSet::const_iterator             CIT_UrbanBlockSet;

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
        
        T_UrbanBlockSet urbanBlocksToNextPointSet_; // The path to the next point pass through these blocks
        T_UrbanBlockSet urbanBlocksOutSet_;         // On this move step, we are out of these blocks
        T_UrbanBlockSet ponctualUrbanBlocksOnSet_;  // Ponctual blocks that are on this move step point (i.e. Objects that aren't on the path to the next point)
    };

    // STL comparison operator : use the SquareDistance between the start pos and the collision pos 
    struct sMoveStepCmp : std::binary_function< const T_MoveStep& , const T_MoveStep&, bool >
    {
    public:
        sMoveStepCmp( const MT_Vector2D& vPosStart )
            : vPosStart_( vPosStart )
        {
        };

        bool operator() ( const T_MoveStep& vPos1, const T_MoveStep& vPos2 ) const
        {
            return vPosStart_.SquareDistance( vPos1.vPos_ ) < vPosStart_.SquareDistance( vPos2.vPos_ );
        };
    private:
        MT_Vector2D vPosStart_;    
    };

    typedef std::set< T_MoveStep, sMoveStepCmp > T_MoveStepSet;
    typedef T_MoveStepSet::iterator              IT_MoveStepSet;
    typedef T_MoveStepSet::const_iterator        CIT_MoveStepSet;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_PathWalker( const DEC_PathWalker& );            //!< Copy constructor
    DEC_PathWalker& operator=( const DEC_PathWalker& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{    
    bool TryToMoveToNextStep        ( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, MT_Float& rTimeRemaining, bool bFirstMove );
    void TryToCrossUrbanBlocks      ( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep );
    bool TryToMoveTo                ( const DEC_PathResult& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining );
    void ComputeObjectsCollision    ( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet );
    void ComputeUrbanBlocksCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet, const std::vector< const urban::TerrainObject_ABC* > blocks );
    void ComputeCurrentSpeed        ();
    void InitializeEnvironment      ( const DEC_PathResult& path );
    bool GoToNextNavPoint           ( const DEC_PathResult& path );
    bool SetCurrentPath             ( boost::shared_ptr< DEC_PathResult > pPath );
    //@}

private:
    PHY_MovingEntity_ABC& movingEntity_;

    TerrainData                       environment_;
    DEC_PathResult::CIT_PathPointList itNextPathPoint_;
    DEC_PathResult::CIT_PathPointList itCurrentPathPoint_;

    // Effect
    MIL_Effect_Move effectMove_;
    MT_Vector2D     vNewPos_;
    MT_Vector2D     vNewDir_;
    MT_Float        rCurrentSpeed_;
    MT_Float        rWalkedDistance_;

    bool            bForcePathCheck_;
    bool            bHasMoved_; 

    boost::shared_ptr< DEC_PathResult > pCurrentPath_;
};


#include "DEC_PathWalker.inl"

#endif // __DEC_PathWalker_h_
