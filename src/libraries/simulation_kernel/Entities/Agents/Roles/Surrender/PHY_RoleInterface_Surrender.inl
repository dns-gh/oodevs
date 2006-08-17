// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 11:02 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Surrender.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Surrender::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Surrender::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}
