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

#ifndef __PHY_MovingEntity_ABC_h_
#define __PHY_MovingEntity_ABC_h_

#include "MIL.h"

#include "Decision/Path/DEC_PathWalker.h"

class MIL_Object_ABC;
class MIL_RealObject_ABC;
class MIL_RC;
class DEC_PathResult;
class TerrainData;

namespace DIN
{
    class DIN_BufferedMessage;
}      

// =============================================================================
// @class  PHY_Actor
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MovingEntity_ABC
{
public:
             PHY_MovingEntity_ABC();
    virtual ~PHY_MovingEntity_ABC();

    //! @name Operations
    //@{
    int  Move         ( DEC_PathResult& path );
    void MoveSuspended( DEC_PathResult& path );
    void MoveCanceled ( DEC_PathResult& path );

    virtual void Clean();
    //@}

    //! @name 
    //@{
    virtual MT_Float GetMaxSpeed              () const = 0;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment ) const = 0;
    virtual MT_Float GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const = 0;
    //@}

    //! @name 
    //@{
    virtual const MT_Vector2D& GetPosition () const = 0;
    virtual const MT_Vector2D& GetDirection() const = 0;

    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float rWalkedDistance ) = 0;
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint   ( const DEC_PathPoint& point ) = 0;
    virtual void NotifyMovingOnSpecialPoint( const DEC_PathPoint& point ) = 0;
    virtual void NotifyMovingInsideObject  ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyMovingOutsideObject ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyEnvironmentChanged  () = 0;
    virtual void NotifyCurrentPathChanged  () = 0;
    //@}

    //! @name 
    //@{
    virtual bool CanMove              () const = 0;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const = 0;
    virtual bool HasResources         () = 0;
    virtual void SendRC               ( const MIL_RC& rc ) const = 0;
    //@}

    //! @name Tools
    //@{
    bool IsMovingOn                  ( const DEC_Path_ABC& path ) const;
    bool ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, const DEC_Knowledge_Object** pObject ) const;
    //@}

protected:
    //! @name Network
    //@{
    void SerializeEnvironmentType( DIN::DIN_BufferedMessage& msg ) const;
    bool SerializeCurrentPath    ( ASN1T_Itineraire& asn         ) const;
    //@}

    //! @name Tools
    //@{
    MT_Vector2D ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const;
    //@}

private:
    DEC_PathWalker pathWalker_;
};

#include "PHY_MovingEntity_ABC.inl"

#endif // __PHY_MovingEntity_ABC_h_
