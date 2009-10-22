// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_MovingEntity_ABC_h_
#define __PHY_MovingEntity_ABC_h_

#include "Decision/Path/DEC_PathWalker.h"
#include <boost/shared_ptr.hpp>

class MIL_Object_ABC;
class MIL_Object_ABC;
class MIL_RC;
class DEC_PathResult;
class TerrainData;

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
    int  Move         ( boost::shared_ptr< DEC_PathResult > pPath );
    void MoveSuspended( boost::shared_ptr< DEC_PathResult > pPath );
    void MoveCanceled ( boost::shared_ptr< DEC_PathResult > pPath );

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
    virtual void SendRC               ( int nReportID ) const = 0;
    //@}

    //! @name Tools
    //@{
    bool IsMovingOn                  ( const DEC_Path_ABC& path ) const;
    bool ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive >
    void serialize( Archive& /*file*/, const uint ) {}
    //@}

protected:
    //! @name Network
    //@{
    void SerializeEnvironmentType( ASN1T_MsgUnitEnvironmentType& msg ) const;
    bool SerializeCurrentPath    ( ASN1T_Path& asn                   ) const;
    //@}

    //! @name Tools
    //@{
    MT_Vector2D ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const;
    //@}

private:
    //! @name Member data
    //@{
    DEC_PathWalker pathWalker_;
    //@}
};

#endif // __PHY_MovingEntity_ABC_h_
