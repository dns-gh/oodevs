// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLE_ACTION_MOVING_H
#define SWORD_ROLE_ACTION_MOVING_H

#include "MIL.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_AgentPion;
class MIL_Object_ABC;

namespace posture
{
    class PostureComputer_ABC;
}

namespace core
{
    class Model;
}

namespace sword
{
// =============================================================================
/** @class  RoleAction_Moving
    @brief  Role action moving
*/
// Created: MCO 2012-02-27
// =============================================================================
class RoleAction_Moving : public moving::PHY_RoleAction_InterfaceMoving
                        , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                        , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             RoleAction_Moving();
             RoleAction_Moving( MIL_AgentPion& pion, const core::Model& entity );
    virtual ~RoleAction_Moving();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;

    void ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double rWalkedDistance );
    //@}

    //! @name Accessors
    //@{
    virtual double GetSpeedWithReinforcement( const TerrainData& environment ) const;
    virtual double GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const;
    virtual double GetMaxSpeedWithReinforcement() const;
    virtual double GetMaxSpeed() const;
    virtual double GetMaxSpeed( const TerrainData& environment ) const;
    virtual double GetMaxSlope() const;
    virtual double GetTheoricSpeedWithReinforcement( const TerrainData& environment ) const;
    virtual double GetTheoricMaxSpeed( bool loaded ) const;
    virtual double GetTheoricMaxSpeedWithReinforcement() const;
    virtual void SetSpeedModificator( double rFactor );
    virtual void SetMaxSpeedModificator( double rFactor );
    virtual double GetMaxSpeedModificator() const;
    virtual bool CanMove() const;
    virtual bool IsReady() const;
    virtual bool HasKnowledgeObject( const MIL_Object_ABC& object ) const;

    bool HasResources();
    virtual void ApplyTrafficModifier();
    //@}

private:
    //! @name Member data
    //@{
    void SetTheoricSpeed( bool ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion*     owner_;
    const core::Model* entity_;
    double             rSpeed_;
    double             rSpeedModificator_;
    double             rMaxSpeedModificator_;
    // Network         
    bool               bHasMove_;
    mutable bool       bTheoricMaxSpeed_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( sword::RoleAction_Moving )

#endif // SWORD_ROLE_ACTION_MOVING_H
