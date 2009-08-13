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

// -----------------------------------------------------------------------------
// Name: MT_RoleContainer::RegisterRole
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
template< typename Role >
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( Role* pRole )
{
	Attach(static_cast< Role::RoleInterface& >( *pRole ));
	this->InterfaceContainer<MT_Role_ABC>::Register ( *pRole) ;
	return static_cast< Role::RoleInterface& >( *pRole );
}

//-----------------------------------------------------------------------------
// Name: MT_RoleContainer::RegisterRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
typename Role::RoleInterface& MT_RoleContainer::RegisterRole()
{
	return RegisterRole(new Role(*this) );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1 > 
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1 )
{
	return RegisterRole( new Role( *this, param1 ) );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2 >
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1, P2& param2 )
{
	return RegisterRole( new Role( *this, param1, param2 ) );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2, typename P3 >
typename Role::RoleInterface& MT_RoleContainer::RegisterRole( P1& param1, P2& param2, P3& param3 )
{
	return RegisterRole( new Role( *this, param1, param2, param3 ) );
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
