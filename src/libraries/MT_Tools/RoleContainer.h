// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/RoleContainer.h $
// $Author: Jvt $
// $Modtime: 5/04/05 11:36 $
// $Revision: 3 $
// $Workfile: RoleContainer.h $
//
// *****************************************************************************

#ifndef __RoleContainer_h_
#define __RoleContainer_h_

#include "MT_Tools/Role_ABC.h"
#include "tools/Extendable.h"
#include "tools/InterfaceContainer.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include <boost/serialization/split_member.hpp>

namespace tools
{

// =============================================================================
// @class  RoleContainer
// Created: JVT/NLD 2004-08-03
// =============================================================================
class RoleContainer : public tools::Extendable< Role_ABC >
                    , public tools::InterfaceContainer<Role_ABC>
{
public:
             RoleContainer() {}
    virtual ~RoleContainer() {}
    
    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< class Archive > void load( Archive& archive, const unsigned int version );
    template< class Archive > void save( Archive& archive, const unsigned int version ) const;
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
    template< typename Role >       typename Role* RetrieveRole()
    {
        return static_cast< Role* >( Retrieve<Role::RoleInterface>() );
    }
    template< typename Role >       typename const Role* RetrieveRole() const
    {
        return static_cast< const Role* >( Retrieve<Role::RoleInterface>() );
    }
    //@}
    template< typename Role > typename Role::RoleInterface& RegisterRole( Role* r);

    //! @name Operation
    //@{
    template< typename Algorithm > const Algorithm& Execute( Algorithm& algorithm );
    //@}
};


#include "RoleContainer.inl"

}

#endif // __RoleContainer_h_
