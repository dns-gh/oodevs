// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_RoleContainer.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 11:22 $
// $Revision: 3 $
// $Workfile: MT_RoleContainer.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::GetIdx
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
inline uint MT_RoleContainer::GetIdx() const
{
    static const uint nIdxForRole = nCurrentIdx_++; // static nIdxForRole will be instanciated for each type 'Role'
    return nIdxForRole;
}


// -----------------------------------------------------------------------------
// Name: MT_RoleContainer::RegisterRole
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
template< typename Role >
void MT_RoleContainer::RegisterRole( Role* pRole )
{
    const uint nIdx = GetIdx< Role::RoleInterface >();
    
    if ( roles_.size() <= nIdx )
        roles_.resize( nIdx + 1, 0 );
    else if ( roles_[ nIdx ] != pRole )
        delete roles_[ nIdx ];
    
    roles_[ nIdx ] = pRole;
}

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::RegisterRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
typename Role::RoleInterface& MT_RoleContainer::RegisterRole()
{
    const uint nIdx = GetIdx< Role::RoleInterface >();

    if( roles_.size() <= nIdx )
        roles_.resize( nIdx + 1, 0 );
    else
        delete roles_[ nIdx ];

    Role* pNewRole = new Role( *this );
    roles_[ nIdx ] = pNewRole;
    return static_cast< Role::RoleInterface& >( *pNewRole );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1 > 
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1 )
{
    const uint nIdx = GetIdx< Role::RoleInterface >();

    if( roles_.size() <= nIdx )
        roles_.resize( nIdx + 1, 0 );
    else
        delete roles_[ nIdx ];

    Role* pNewRole = new Role( *this, param1 );
    roles_[ nIdx ] = pNewRole;
    return static_cast< Role::RoleInterface& >( *pNewRole );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2 >
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1, P2& param2 )
{
    const uint nIdx = GetIdx< Role::RoleInterface >();

    if( roles_.size() <= nIdx )
        roles_.resize( nIdx + 1, 0 );
    else
        delete roles_[ nIdx ];

    Role* pNewRole = new Role( *this, param1, param2 );
    roles_[ nIdx ] = pNewRole;
    return static_cast< Role::RoleInterface& >( *pNewRole );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2, typename P3 >
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1, P2& param2, P3& param3 )
{
    const uint nIdx = GetIdx< Role::RoleInterface >();

    if( roles_.size() <= nIdx )
        roles_.resize( nIdx + 1, 0 );
    else
        delete roles_[ nIdx ];

    Role* pNewRole = new Role( *this, param1, param2, param3 );
    roles_[ nIdx ] = pNewRole;
    return static_cast< Role::RoleInterface& >( *pNewRole );
}

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::GetRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role >
typename Role::RoleInterface& MT_RoleContainer::GetRoleInterface()
{ 
    const uint nIdx = GetIdx< Role::RoleInterface >();
    if( roles_.size() <= nIdx || roles_[ nIdx ] == 0 )
        RegisterRole< Role::RoleInterface >();
    return static_cast< Role::RoleInterface& >( *roles_[ nIdx ] ); 
}

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::GetRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
const typename Role::RoleInterface& MT_RoleContainer::GetRoleInterface() const  
{ 
    const uint nIdx = GetIdx< Role::RoleInterface >();
    if( roles_.size() <= nIdx || roles_[ nIdx ] == 0 )
        RegisterRole< Role::RoleInterface >();
    return static_cast< const Role& >( *roles_[ nIdx ] ); 
}


//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::GetRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role >
typename Role& MT_RoleContainer::GetRole()
{ 
    const uint nIdx = GetIdx< Role::RoleInterface >();
    assert( roles_.size() > nIdx && roles_[ nIdx ] != 0 );
    return static_cast< Role& >( *roles_[ nIdx ] ); 
}

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::GetRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
const typename Role& MT_RoleContainer::GetRole() const  
{ 
    const uint nIdx = GetIdx< Role::RoleInterface >();
    assert( roles_.size() > nIdx && roles_[ nIdx ] != 0 );
    return static_cast< const Role& >( *roles_[ nIdx ] ); 
}

// -----------------------------------------------------------------------------
// Name: MT_RoleContainer::serialize
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_RoleContainer::serialize( Archive&, const uint )
{
}
