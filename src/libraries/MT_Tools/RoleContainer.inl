// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/RoleContainer.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 11:22 $
// $Revision: 3 $
// $Workfile: RoleContainer.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: RoleContainer::RegisterRole
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
template< typename Role >
typename Role::RoleInterface& RoleContainer::RegisterRole( Role& role )
{
    Attach( static_cast< Role::RoleInterface& >( role ) );
    InterfaceContainer<Role_ABC>::Register( role );
    return static_cast< Role::RoleInterface& >( role );
}

// -----------------------------------------------------------------------------
// Name: RoleContainer::load
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< class Archive >
void RoleContainer::load( Archive& /*archive*/, const unsigned int /*version*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleContainer::save
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< class Archive >
void RoleContainer::save( Archive& /*archive*/, const unsigned int /*version*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleContainer::Execute
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< typename Algorithm >
const Algorithm& RoleContainer::Execute( Algorithm& algorithm )
{
    Apply( & AlgorithmModifier_ABC< Algorithm >::Execute, algorithm );
    return algorithm;
}

#define CALL_ROLE_DEF( z, n, d ) \
    template< typename Role BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > void RoleContainer::CallRole( void (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ) BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) \
    { \
        Role* role = RoleContainer::RetrieveRole< Role >(); \
        if( role ) \
        { \
            (role->*func)( BOOST_PP_ENUM_PARAMS( n, p ) ); \
        } \
    }

#define CALL_ROLE_DEF_CONST( z, n, d ) \
    template< typename Role BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > void RoleContainer::CallRole( void (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ) const BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) const \
    { \
        const Role* role = RoleContainer::RetrieveRole< Role >(); \
        if( role ) \
        { \
            (role->*func)( BOOST_PP_ENUM_PARAMS( n, p ) ); \
        } \
    }

#define CALL_ROLE_RETVAL_DEF( z, n, d ) \
    template< typename Role, typename RetType BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > RetType RoleContainer::CallRole( RetType (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A ) ), const RetType& defVal BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) \
    { \
        Role* role = RoleContainer::RetrieveRole< Role >(); \
        if( role ) \
        { \
           return  (role->*func)( BOOST_PP_ENUM_PARAMS( n, p ) ); \
        } \
        return defVal; \
    }

#define CALL_ROLE_DEF_RETVAL_CONST( z, n, d ) \
    template< typename Role, typename RetType BOOST_PP_ENUM_TRAILING_PARAMS(n, typename A ) > RetType RoleContainer::CallRole( RetType (Role::*func)( BOOST_PP_ENUM_PARAMS( n, A) ) const, const RetType& defVal BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( n, BOOST_CONST_REF_PRE( A ), BOOST_CONST_REF_POST p ) ) const \
    { \
        const Role* role = RoleContainer::RetrieveRole< Role >(); \
        if( role ) \
        { \
            return (role->*func)( BOOST_PP_ENUM_PARAMS( n, p ) ); \
        } \
        return defVal; \
    }

BOOST_PP_REPEAT( 5, CALL_ROLE_DEF, l )
BOOST_PP_REPEAT( 5, CALL_ROLE_DEF_CONST, l )
BOOST_PP_REPEAT( 5, CALL_ROLE_RETVAL_DEF, l )
BOOST_PP_REPEAT( 5, CALL_ROLE_DEF_RETVAL_CONST, l )

#undef CALL_ROLE_DEF
#undef CALL_ROLE_DEF_CONST
#undef CALL_ROLE_RETVAL_DEF
#undef CALL_ROLE_DEF_RETVAL_CONST
