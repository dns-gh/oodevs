// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:10 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Refugee_h_
#define __PHY_RoleInterface_Refugee_h_

#include "MT_Tools/MT_Role_ABC.h"

class MIL_AgentPion;
class MIL_Object_ABC;

// =============================================================================
// @class  PHY_RoleInterface_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Refugee : public MT_Role_ABC
                                , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Refugee RoleInterface;

public:
    explicit PHY_RoleInterface_Refugee( MT_RoleContainer& role );
             PHY_RoleInterface_Refugee();
    virtual ~PHY_RoleInterface_Refugee();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    virtual bool IsManaged() const = 0;
    virtual bool IsManaged( const MIL_Object_ABC& camp ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool Orientate( const MIL_AgentPion& pionManaging ) = 0;
    virtual bool Release  () = 0;
    virtual bool Release  ( const MIL_Object_ABC& camp ) = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Refugee::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Refugee::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Refugee_h_
