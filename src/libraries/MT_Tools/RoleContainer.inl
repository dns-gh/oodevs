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
