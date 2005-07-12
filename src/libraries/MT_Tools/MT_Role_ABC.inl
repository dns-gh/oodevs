// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Role_ABC.inl $
// $Author: Jvt $
// $Modtime: 5/04/05 10:19 $
// $Revision: 2 $
// $Workfile: MT_Role_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MT_Role_ABC::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
template< typename Archive > 
void MT_Role_ABC::serialize( Archive& file , const uint )
{
    file & pRoleContainer_;
}