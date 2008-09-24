// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Role_ABC.h $
// $Author: Jvt $
// $Modtime: 31/03/05 14:44 $
// $Revision: 3 $
// $Workfile: MT_Role_ABC.h $
//
// *****************************************************************************

#ifndef __MT_Role_ABC_h_
#define __MT_Role_ABC_h_

#include "MT_RoleContainer.h"

// =============================================================================
// @class  MT_Role_ABC
// Created: JVT/NLD 2004-08-03
// =============================================================================
class MT_Role_ABC
{
    MT_COPYNOTALLOWED( MT_Role_ABC )

public:
             MT_Role_ABC( MT_RoleContainer& roleContainer );
             MT_Role_ABC();
    virtual ~MT_Role_ABC() = 0;
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
protected:
    //! @name Role accessors
    //@{
    template< typename Role >       typename Role/*::RoleInterface*/& GetRole()        { assert( pRoleContainer_ ); return pRoleContainer_->GetRole< Role >(); }
    template< typename Role > const typename Role/*::RoleInterface*/& GetRole() const  { assert( pRoleContainer_ ); return pRoleContainer_->GetRole< Role >(); }

    template< typename Role >       typename Role::RoleInterface& GetRoleInterface()        { assert( pRoleContainer_ ); return pRoleContainer_->GetRoleInterface< Role >(); }
    template< typename Role > const typename Role::RoleInterface& GetRoleInterface() const  { assert( pRoleContainer_ ); return pRoleContainer_->GetRoleInterface< Role >(); }
    //@}

private:
    MT_RoleContainer* pRoleContainer_;
};

#include "MT_Role_ABC.inl"

#endif // __MT_Role_ABC_h_