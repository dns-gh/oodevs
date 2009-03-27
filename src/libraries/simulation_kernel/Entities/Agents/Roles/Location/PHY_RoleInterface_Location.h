// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:59 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Location.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Location_h_
#define __PHY_RoleInterface_Location_h_

#include "simulation_kernel/MIL.h"
#include "MT_Tools/MT_Role_ABC.h"
#include "simulation_terrain/TER_Agent_ABC.h"

class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_Object_ABC;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;

// =============================================================================
// @class  PHY_RoleInterface_Location
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Location : public MT_Role_ABC
                                 , public TER_Agent_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Location )

public:
    typedef PHY_RoleInterface_Location RoleInterface;

public:
             PHY_RoleInterface_Location( MT_RoleContainer& role );
             PHY_RoleInterface_Location();
    virtual ~PHY_RoleInterface_Location();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual void NotifyPopulationCollision( MIL_PopulationFlow&          population ) = 0;
    virtual void NotifyPopulationCollision( MIL_PopulationConcentration& population ) = 0;
    virtual void NotifyObjectCollision    ( MIL_Object_ABC& object ) = 0;

    virtual void NotifyMovingInsideObject ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyMovingOutsideObject( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutInsideObject    ( MIL_Object_ABC& object ) = 0;
    virtual void NotifyPutOutsideObject   ( MIL_Object_ABC& object ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float           GetHeight       () const = 0;
    virtual MT_Float           GetAltitude     () const = 0;
    virtual const MT_Vector2D& GetPosition     () const = 0;
    virtual const MT_Vector2D& GetDirection    () const = 0;
    virtual MIL_Agent_ABC&     GetAgent        () const = 0;
    virtual MT_Float           GetCurrentSpeed () const = 0;
    virtual bool               HasDoneMagicMove() const = 0;
    //@}
};

#include "PHY_RoleInterface_Location.inl"

#endif // __PHY_RoleInterface_Location_h_
