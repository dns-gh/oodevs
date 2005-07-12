// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 11:02 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Perceiver.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Perceiver::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleInterface_Perceiver::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}
