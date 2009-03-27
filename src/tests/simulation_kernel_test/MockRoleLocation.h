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

#include "MT_Tools/MT_RoleContainer.h"
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
    MockRoleLocation( MT_RoleContainer& container ) 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRoleLocation" ) )
        , PHY_RoleInterface_Location( container )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetHeight )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetAltitude )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetCurrentSpeed )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( HasDoneMagicMove )  
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetAgentShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetPositionShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetDirectionShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPopulationCollision_Flow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPopulationCollision_Concentration )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyObjectCollision )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyMovingInsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyMovingOutsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPutInsideObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyPutOutsideObject )
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

    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyObjectCollision, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyMovingInsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyMovingOutsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyPutInsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockRoleLocation, NotifyPutOutsideObject, MIL_Object_ABC&, , MIL_Object_ABC );
};

#endif // __MockRoleLocation_h_
