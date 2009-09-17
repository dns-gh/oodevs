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
typename Role::RoleInterface& RoleContainer::RegisterRole( Role* pRole )
{
	Attach( static_cast< Role::RoleInterface& >( *pRole ) );
	InterfaceContainer<Role_ABC>::Register( *pRole ) ;
	return static_cast< Role::RoleInterface& >( *pRole );
}

//-----------------------------------------------------------------------------
// Name: RoleContainer::RegisterRole
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
template< typename Role > 
typename Role::RoleInterface& RoleContainer::RegisterRole()
{
	return RegisterRole( new Role() );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1 > 
typename Role::RoleInterface& RoleContainer::RegisterRole( P1& param1 )
{
	return RegisterRole( new Role( param1 ) );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2 >
typename Role::RoleInterface& RoleContainer::RegisterRole( P1& param1, P2& param2 )
{
	return RegisterRole( new Role( param1, param2 ) );
}

// -----------------------------------------------------------------------------
// Name: 
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
template< typename Role, typename P1, typename P2, typename P3 >
typename Role::RoleInterface& RoleContainer::RegisterRole( P1& param1, P2& param2, P3& param3 )
{
	return RegisterRole( new Role( param1, param2, param3 ) );
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void RoleContainer::load
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< class Archive >
void RoleContainer::load( Archive& archive, const unsigned int /*version*/ )
{
    std::vector< Role_ABC* > container;
    archive >> container;
}

// -----------------------------------------------------------------------------
// Name: template< class Archive > void RoleContainer::save
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< class Archive >
void RoleContainer::save( Archive& archive, const unsigned int /*version*/ ) const
{
    archive << extensions_.Container();
}

// -----------------------------------------------------------------------------
// Name: template< typename Algorithm > void Execute( Algorithm& algorithm )
// Created: SBO 2009-08-14
// -----------------------------------------------------------------------------
template< typename Algorithm >
void RoleContainer::Execute( Algorithm& algorithm )
{
    Apply( & AlgorithmModifier_ABC< Algorithm >::Execute, algorithm );
}

