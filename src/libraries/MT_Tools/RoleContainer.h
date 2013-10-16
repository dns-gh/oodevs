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
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include <tools/InterfaceContainer.h>
#include <tools/Extendable.h>
#include <boost/serialization/split_member.hpp>
#include <boost/preprocessor.hpp>
#include <boost/type_traits.hpp>

#define BOOST_CONST_REF_PRE( A ) typename boost::add_lvalue_reference< typename boost::add_const< A
#define BOOST_CONST_REF_POST >::type >::type

#define CALL_ROLE_DECL( z, n, d ) \
     template< typename Role BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > void CallRole( void (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ) BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) );

#define CALL_ROLE_DECL_CONST( z, n, d ) \
     template< typename Role BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > void CallRole( void (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ) const BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) const;

#define CALL_ROLE_RETVAL_DECL( z, n, d ) \
     template< typename Role, typename RetType BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > RetType CallRole( RetType (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ), const RetType& defVal BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) );

#define CALL_ROLE_RETVAL_DECL_CONST( z, n, d ) \
     template< typename Role, typename RetType BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > RetType CallRole( RetType (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ) const, const RetType& defVal BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) const;

namespace tools
{
// =============================================================================
// @class  RoleContainer
// Created: JVT/NLD 2004-08-03
// =============================================================================
class RoleContainer : public tools::Extendable< Role_ABC >
                    , public tools::InterfaceContainer< Role_ABC >
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

    //! @name Role accessors
    //@{
    template< typename Role > typename Role& GetRole()
    {
        return static_cast< Role& >( Get<Role::RoleInterface>() );
    }
    template< typename Role > const typename Role& GetRole() const
    {
          return static_cast< const Role& >( Get<Role::RoleInterface>() );
    }
    template< typename Role > typename Role* RetrieveRole()
    {
        return static_cast< Role* >( Retrieve<Role::RoleInterface>() );
    }
    template< typename Role > typename const Role* RetrieveRole() const
    {
        return static_cast< const Role* >( Retrieve<Role::RoleInterface>() );
    }
    //@}
    template< typename Role > typename Role::RoleInterface& RegisterRole( Role& r );

    //! @name Operation
    //@{
    template< typename Algorithm > const Algorithm& Execute( Algorithm& algorithm );

    BOOST_PP_REPEAT( 5, CALL_ROLE_DECL, l )
    BOOST_PP_REPEAT( 5, CALL_ROLE_DECL_CONST, l )
    BOOST_PP_REPEAT( 5, CALL_ROLE_RETVAL_DECL, l )
    BOOST_PP_REPEAT( 5, CALL_ROLE_RETVAL_DECL_CONST, l )
    //@}
};

#include "RoleContainer.inl"

#undef BOOST_CONST_REF_PRE
#undef BOOST_CONST_REF_POST
#undef CALL_ROLE_DECL
#undef CALL_ROLE_DECL_CONST
#undef CALL_ROLE_RETVAL_DECL
#undef CALL_ROLE_RETVAL_DECL_CONST
}

#endif // __RoleContainer_h_
