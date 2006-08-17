// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 11:02 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Refugee.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Refugee::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Refugee::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}