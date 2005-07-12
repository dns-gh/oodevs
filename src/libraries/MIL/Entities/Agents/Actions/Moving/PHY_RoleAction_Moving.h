// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h $
// $Author: Age $
// $Modtime: 13/04/05 18:25 $
// $Revision: 14 $
// $Workfile: PHY_RoleAction_Moving.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Moving_h_
#define __PHY_RoleAction_Moving_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

#include "Decision/Path/DEC_Path.h"
#include "Entities/Effects/MIL_Effect_Move.h"

class PHY_RolePion_Location;
class MIL_AgentPion;
class MIL_Object_ABC;
class MIL_RealObject_ABC;
class MIL_RealObjectTypeFilter;

// =============================================================================
// @class  PHY_RoleAction_Moving
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Moving : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Moving )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Moving RoleInterface;

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
             PHY_RoleAction_Moving( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RoleAction_Moving();
    virtual ~PHY_RoleAction_Moving();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Operations
    //@{
    MT_Float GetMaxSlope() const;
    MT_Float GetMaxSpeedWithReinforcement() const;
    MT_Float GetSpeedWithReinforcement   ( const TerrainData& nEnv ) const;
    void     SetSpeedModificator         ( MT_Float rFactor );
    void     SetMaxSpeedModificator      ( MT_Float rFactor );
    int      Move                        ( DEC_Path& path );
    void     MoveSuspended               ( DEC_Path& path );
    void     MoveCanceled                ( DEC_Path& path );

    void Apply(); // Called by MIL_Effect_Move
    //@}

    //! @name Network
    //@{
    void SendChangedState();
    void SendFullState   ();
    //@}

    //! @name Path operations
    //@{
        MT_Vector2D ExtrapolatePosition          ( MT_Float rTime, bool bBoundOnPath ) const;
        void        ComputeFutureObjectCollisions( const MIL_RealObjectTypeFilter& objectsFilter, DEC_Path::T_KnowledgeObjectMultimap& objectsOnPath ) const;
    //@}
    
    //! @name Accessors
    //@{
    bool IsMovingOn( const DEC_Path& path ) const;
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
    //! @name Tools
    //@{    
    bool TryToMoveToNextStep    ( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, MT_Float& rTimeRemaining, bool bFirstMove );
    bool TryToMoveTo            ( const DEC_Path& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining );
    void ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet );
    void ComputeCurrentSpeed    ();
    void InitializeEnvironment  ( const DEC_Path& path );
    bool SetCurrentPath         (       DEC_Path& path );
    bool GoToNextNavPoint       ( const DEC_Path& path );
    bool CanMove                () const;
    bool ReserveConsumptionWithReinforcement();
    //@}

    //! @name Speed management
    //@{
    MT_Float GetMaxSpeed              () const;
    MT_Float GetMaxSpeed              ( const TerrainData& nEnv ) const;
    MT_Float GetMaxSpeed              ( const MIL_RealObject_ABC& object ) const;
    MT_Float ApplyMaxSpeedModificators( MT_Float rSpeed ) const;
    MT_Float ApplySpeedModificators   ( MT_Float rSpeed ) const;

    MT_Float GetSpeedWithReinforcement( const MIL_Object_ABC& object ) const;
    //@}

private:
    MIL_AgentPion*              pPion_;
    PHY_RolePion_Location*      pRoleLocation_;
    MT_Float                    rSpeedModificator_;
    MT_Float                    rMaxSpeedModificator_;
    TerrainData                 nEnvironment_;
    DEC_Path::CIT_PathPointList itNextPathPoint_;
    DEC_Path::CIT_PathPointList itCurrentPathPoint_;

    // Effect
    MIL_Effect_Move effectMove_;
    MT_Vector2D     vNewPos_;
    MT_Vector2D     vNewDir_;
    MT_Float        rCurrentSpeed_;

    MT_Float        rCurrentMaxSpeed_;      // Cached data
    MT_Float        rCurrentEnvSpeed_;

    bool            bForcePathCheck_;
    bool            bHasChanged_;
    bool            bHasMoved_; 

    DEC_Path*       pCurrentPath_; // Toujours valide : le role fait un IncRef() / DecRef() tant qu'il veut garder une reference dessus
};

#include "PHY_RoleAction_Moving.inl"

#endif // __PHY_RoleAction_Moving_h_
