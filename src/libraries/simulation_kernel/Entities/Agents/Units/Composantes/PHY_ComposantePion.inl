// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposantePion.inl $
// $Author: Jvt $
// $Modtime: 11/05/05 14:49 $
// $Revision: 15 $
// $Workfile: PHY_ComposantePion.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_ComposantePion::FillFireData
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_ComposantePion::ApplyOnWeapons( T& functor ) const
{
    for( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
        functor( *this, **itWeapon );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_ComposantePion::FillFireData
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T > 
inline void PHY_ComposantePion::ApplyOnHumanProtection( T& functor ) const
{
    for( CIT_HumanProtectionVector it = protections_.begin(); it != protections_.end(); ++it )
        functor( *this, **it );
}


// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnSensors
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposantePion::ApplyOnSensors( T& func ) const
{
    for( CIT_SensorVector itSensor = sensors_.begin(); itSensor != sensors_.end(); ++itSensor )
        func( **itSensor );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnRadars
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposantePion::ApplyOnRadars( T& func ) const
{
    GetType().ApplyOnRadars( func );
}

