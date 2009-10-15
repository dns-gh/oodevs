// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleImplantable_h_
#define __MockRoleImplantable_h_

#include "simulation_kernel/Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
//#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"

// =============================================================================
/** @class  MockRoleImplantable
    @brief  MockRoleImplantable
*/
// Created: MGD 2009-03-04
// =============================================================================
class MockRoleInterface_Posture
    : public mockpp::ChainableMockObject
    , public PHY_RoleInterface_Posture
{
public:
    
    //! @name Constructors/Destructor
    //@{
    MockRoleInterface_Posture( )
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRoleImplantable" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetLastPostureShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetCurrentPostureShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetPostureCompletionPercentage )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CanBePerceived )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetPosturePostePrepareGenie )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnsetPosturePostePrepareGenie )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetTimingFactor )
    {
    }

    virtual ~MockRoleInterface_Posture() {}
    //@}

    virtual const PHY_Posture& GetLastPosture() const
    {
        return *GetLastPostureShadow();
    }

    virtual const PHY_Posture& GetCurrentPosture() const
    {
        return *GetCurrentPostureShadow();
    }



    MOCKPP_CONST_CHAINABLE_EXT0( MockRoleInterface_Posture, const PHY_Posture*, GetLastPostureShadow, PHY_Posture, );
    MOCKPP_CONST_CHAINABLE_EXT0( MockRoleInterface_Posture, const PHY_Posture*, GetCurrentPostureShadow, PHY_Posture, );

    MOCKPP_CONST_CHAINABLE0( MockRoleInterface_Posture, MT_Float, GetPostureCompletionPercentage );

    MOCKPP_CONST_CHAINABLE_EXT1( MockRoleInterface_Posture, bool, CanBePerceived, const MIL_AgentPion&, bool, , MIL_AgentPion );

    MOCKPP_VOID_CHAINABLE0( MockRoleInterface_Posture, SetPosturePostePrepareGenie );
    MOCKPP_VOID_CHAINABLE0( MockRoleInterface_Posture, UnsetPosturePostePrepareGenie );

    MOCKPP_VOID_CHAINABLE_EXT1( MockRoleInterface_Posture, SetTimingFactor, MT_Float, , MT_Float );



    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) {};
    virtual void Clean     () {};
    virtual bool HasChanged() const { return false; };

    // Override automatic postures
    virtual void EnableDiscreteMode () {};
    virtual void DisableDiscreteMode() {};
    //@}

    //! @name Installation
    //@{
    virtual bool IsInstalled  () const { return false; };
    virtual bool IsUninstalled() const { return false;};
    virtual void Install      () {};
    //@}

    //! @name Perception
    //@{
    virtual void SetStealthFactor( MT_Float rValue ) {};
    //@}

    //! @name Elongation
    //@{
    virtual void     SetElongationFactor( MT_Float ) {};
    virtual MT_Float GetElongationFactor() const { return 0; };
    //@}

    //! @name Accessors
    //@{
    virtual bool IsStealth() const { return false; };
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const {};
    virtual bool HLAStatusHasChanged() const { return false; };
};

#endif // __MockRoleImplantable_h_
