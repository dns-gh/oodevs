// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __DEC_PathWalker_h_
#define __DEC_PathWalker_h_

#include "MIL.h"

#include "DEC_PathResult.h"
#include "Entities/Effects/MIL_Effect_Move.h"

class MIL_Object_ABC;
class TerrainData;
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
        eItineraireMustBeJoined
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     DEC_PathWalker( PHY_MovingEntity_ABC& movingEntity );
    ~DEC_PathWalker();
    //@}

    //! @name Operations
    //@{
    int  Move         ( DEC_PathResult& path );
    void MoveSuspended( DEC_PathResult& path );
    void MoveCanceled ( DEC_PathResult& path );

    void Apply(); // Called by MIL_Effect_Move

    void Clean();
    //@}

    //! @name Tools
    //@{
    bool        ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, const DEC_Knowledge_Object** pObject ) const;
    MT_Vector2D ExtrapolatePosition         ( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const;
    bool        IsMovingOn                  ( const DEC_Path_ABC& path ) const;
    //@}

    //! @name Network
    //@{
    void SerializeEnvironmentType( DIN::DIN_BufferedMessage& msg ) const;
    bool SerializeCurrentPath    ( ASN1T_Itineraire& asn         ) const;
    //@}

private:
     //! @name Types
    //@{
    typedef std::set< MIL_Object_ABC* > T_ObjectSet;
    typedef T_ObjectSet::const_iterator CIT_ObjectSet;

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
    bool TryToMoveToNextStep    ( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, MT_Float& rTimeRemaining, bool bFirstMove );
    bool TryToMoveTo            ( const DEC_PathResult& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining );
    void ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet );
    void ComputeCurrentSpeed    ();
    void InitializeEnvironment  ( const DEC_PathResult& path );
    bool SetCurrentPath         (       DEC_PathResult& path );
    bool GoToNextNavPoint       ( const DEC_PathResult& path );
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

    bool            bForcePathCheck_;
    bool            bHasMoved_; 

    DEC_PathResult* pCurrentPath_; // Toujours valide : le role fait un IncRef() / DecRef() tant qu'il veut garder une reference dessus
};


#include "DEC_PathWalker.inl"

#endif // __DEC_PathWalker_h_
