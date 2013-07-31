// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 18:28 $
// $Revision: 5 $
// $Workfile: PHY_RolePion_Composantes.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyOnWeapons
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_RolePion_Composantes::ApplyOnWeapons( T& t ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        ( **it ).ApplyOnWeapons( t );
}
