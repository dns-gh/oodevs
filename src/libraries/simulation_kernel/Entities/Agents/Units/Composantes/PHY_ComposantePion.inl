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
// Name: PHY_ComposantePion::FillFireData
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T >
inline void PHY_ComposantePion::ApplyOnWeapons( T& functor ) const
{
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        functor( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: void PHY_ComposantePion::ApplyOnWeapons
// Created: MCO 2012-09-05
// -----------------------------------------------------------------------------
template< typename T > void PHY_ComposantePion::ApplyOnWeapons( const T& functor ) const
{
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        functor( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnHumanProtection
// Created: NLD 2006-08-07
// -----------------------------------------------------------------------------
template< typename T >
inline void PHY_ComposantePion::ApplyOnHumanProtection( T& functor ) const
{
    for( auto it = protections_.begin(); it != protections_.end(); ++it )
        functor( *this, **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnSensors
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposantePion::ApplyOnSensors( T& func ) const
{
    for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
        func( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnSensors
// Created: MCO 2012-09-05
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ComposantePion::ApplyOnSensors( const T& func ) const
{
    for( auto it = sensors_.begin(); it != sensors_.end(); ++it )
        func( **it );
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

// -----------------------------------------------------------------------------
// Name: PHY_ComposantePion::ApplyOnRadars
// Created: MCO 2012-09-05
// -----------------------------------------------------------------------------
template< typename T > void PHY_ComposantePion::ApplyOnRadars( const T& func ) const
{
    GetType().ApplyOnRadars( func );
}
