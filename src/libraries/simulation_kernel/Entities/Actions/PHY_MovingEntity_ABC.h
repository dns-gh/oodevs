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

#include "Knowledge/DEC_Knowledge_Def.h"
#include <memory>

namespace sword
{
    class Path;
    class UnitEnvironmentType;
}

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class DEC_Knowledge_Object;
class DEC_Path_ABC;
class DEC_PathPoint;
class DEC_PathResult;
class DEC_PathType;
class DEC_PathWalker;
class TerrainData;
struct MIL_DecisionalReport;

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
    bool HasCurrentPath() const;
    virtual void Clean();
    //@}

    //! @name
    //@{
    virtual double GetSpeed( const TerrainData& environment ) const = 0;
    virtual double GetSpeed( const TerrainData& environment, const MIL_Object_ABC& object ) const = 0;
    virtual double GetSlopeDeceleration() const = 0;
    virtual double GetMaxSlope() const = 0;
    //@}

    //! @name
    //@{
    virtual const MT_Vector2D& GetPosition () const = 0;
    virtual const MT_Vector2D& GetDirection() const = 0;

    virtual void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance ) = 0;
    //@}

    //! @name Notifications
    //@{
    virtual void NotifyMovingOnPathPoint   ( const MT_Vector2D& point ) = 0;
    virtual void NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point ) = 0;
    virtual void NotifyMovingInsideObject  ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyMovingOutsideObject ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyEnvironmentChanged  () = 0;
    virtual void NotifyCurrentPathChanged  () = 0;
    //@}

    //! @name
    //@{
    virtual bool CanMove              () const = 0;
    virtual bool IsReady              () const = 0;
    virtual bool CanObjectInteractWith( const MIL_Object_ABC& object ) const = 0;
    virtual bool HasKnowledgeObject   ( const MIL_Object_ABC& object ) const = 0;
    virtual double GetObjectCost      ( const MIL_ObjectType_ABC& objectType, const DEC_PathType& pathType ) const = 0;
    virtual bool HasResources         () = 0;
    virtual void SendRC               ( const MIL_DecisionalReport& reportId ) const = 0;
    virtual void SendRC               ( const MIL_DecisionalReport& reportId, const std::string& name ) const = 0;
    //@}

    //! @name Tools
    //@{
    bool IsMovingOn                  ( const DEC_Path_ABC& path ) const;
    bool ComputeFutureObjectCollision( const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive >
    void serialize( Archive& /*file*/, const unsigned int ) {}
    void SerializeCurrentPath( sword::Path& asn ) const;
    //@}

protected:
    //! @name Network
    //@{
    void SerializeEnvironmentType( sword::UnitEnvironmentType& msg ) const;
    //@}

    //! @name Tools
    //@{
    MT_Vector2D ExtrapolatePosition( const MT_Vector2D& position, const double rSpeed, const double rTime, const bool bBoundOnPath ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< DEC_PathWalker > pathWalker_;
    //@}
};

#endif // __PHY_MovingEntity_ABC_h_
