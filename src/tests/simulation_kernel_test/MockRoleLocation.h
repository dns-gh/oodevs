// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleLocation_h_
#define __MockRoleLocation_h_

#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

// =============================================================================
/** @class  MockRoleLocation
    @brief  MockRoleLocation
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockRoleLocation
    : public mockpp::ChainableMockObject
    , public PHY_RoleInterface_Location
{
public:
    
    //! @name Constructors/Destructor
    //@{
    MockRoleLocation( )
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRoleLocation" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetHeight )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetAltitude )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetCurrentSpeed )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( HasDoneMagicMove )  
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetAgentShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetPositionShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetDirectionShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPopulationCollision_Flow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPopulationCollision_Concentration )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyTerrainObjectCollision )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyMovingInsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyMovingOutsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyTerrainPutInsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyTerrainPutOutsideObject )
    {
    }
    virtual ~MockRoleLocation() {}
    //@}
        
    virtual MIL_Agent_ABC&     GetAgent() const 
    {
        return *GetAgentShadow();
    }
    virtual const MT_Vector2D& GetPosition() const
    {
        return *GetPositionShadow();
    }
    virtual const MT_Vector2D& GetDirection() const
    {
        return *GetDirectionShadow();
    }

    MOCKPP_CONST_CHAINABLE0          ( MockRoleLocation, MT_Float, GetHeight );
    MOCKPP_CONST_CHAINABLE0          ( MockRoleLocation, MT_Float, GetAltitude );
    MOCKPP_CONST_CHAINABLE0          ( MockRoleLocation, MT_Float, GetCurrentSpeed );
    MOCKPP_CONST_CHAINABLE0          ( MockRoleLocation, bool, HasDoneMagicMove );


    MOCKPP_CONST_CHAINABLE_EXT0      ( MockRoleLocation, const MT_Vector2D*, GetPositionShadow, MT_Vector2D, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockRoleLocation, const MT_Vector2D*, GetDirectionShadow, MT_Vector2D, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockRoleLocation, MIL_Agent_ABC*, GetAgentShadow, MIL_Agent_ABC, );

    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyPopulationCollision, MIL_PopulationFlow&, _Flow, MIL_PopulationFlow );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyPopulationCollision, MIL_PopulationConcentration&, _Concentration, MIL_PopulationConcentration );

    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyTerrainObjectCollision, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyMovingInsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyMovingOutsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyTerrainPutInsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyTerrainPutOutsideObject, MIL_Object_ABC&, , MIL_Object_ABC );

    virtual void Update( bool bIsDead ) {};
    virtual void Clean () {};

    virtual boost::shared_ptr<MT_Vector2D> GetSharedPosition() const { throw; };

    virtual void Hide     () {};
    virtual void Show     ( const MT_Vector2D& vNewPosition ) {};
    virtual void MagicMove( const MT_Vector2D& vNewPosition ) {};
    virtual void Move     ( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed ) {};
    virtual void Follow   ( const MIL_Agent_ABC& agent ) {};
    virtual void Fly      ( MT_Float rHeight ) {};
    virtual bool               HasSpeedChanged      () const { return false; }; // Position or direction or height has changed 
    virtual bool               HasLocationChanged   () const { return false; };
    virtual void Serialize( HLA_UpdateFunctor& functor ) const {};
};

#endif // __MockRoleLocation_h_
