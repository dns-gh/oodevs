// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_RoleContainer.h $
// $Author: Jvt $
// $Modtime: 5/04/05 11:36 $
// $Revision: 3 $
// $Workfile: MT_RoleContainer.h $
//
// *****************************************************************************

#ifndef __MT_RoleContainer_h_
#define __MT_RoleContainer_h_

#include "MT_Tools_Types.h"
#include <boost/shared_ptr.hpp>
#include "tools/Extendable.h"
#include "tools/InterfaceContainer.h"

class MT_Role_ABC; // FIXME(warning C4150) AHC : A remplacer par #include "MT_Role_ABC.h" quand MT_Role_ABC::GetRole sera supprime

// =============================================================================
// @class  MT_RoleContainer
// Created: JVT/NLD 2004-08-03
// =============================================================================
class MT_RoleContainer : public tools::Extendable<MT_Role_ABC>,
						 public tools::InterfaceContainer<MT_Role_ABC>
{
public:
             MT_RoleContainer();
    virtual ~MT_RoleContainer();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Role registration
    //@{
    template< typename Role >                                        typename Role::RoleInterface& RegisterRole();
    template< typename Role, typename P1 >                           typename Role::RoleInterface& RegisterRole( P1& );
    template< typename Role, typename P1, typename P2  >             typename Role::RoleInterface& RegisterRole( P1&, P2& );
    template< typename Role, typename P1, typename P2, typename P3 > typename Role::RoleInterface& RegisterRole( P1&, P2&, P3& );
    //@}

    //! @name Role accessors
    //@{
    template< typename Role >       typename Role& GetRole()
    {
    	return static_cast< Role& >( Get<Role::RoleInterface>() );
    }
    template< typename Role > const typename Role& GetRole() const
    {
      	return static_cast< const Role& >( Get<Role::RoleInterface>() );
    }
    //@}
protected:
    template< typename Role > typename Role::RoleInterface& RegisterRole( Role* r);
};

#include "MT_RoleContainer.inl"

#endif // __MT_RoleContainer_h_
