// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.inl $
// $Author: Jvt $
// $Modtime: 1/04/05 11:25 $
// $Revision: 3 $
// $Workfile: PHY_HumansComposante.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_HumansComposante::serialize( Archive& file, const uint )
{
    file & pComposante_
         & humans_
         & nNbrUsableHumans_;
    assert( pComposante_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetOperationalState
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
inline
MT_Float PHY_HumansComposante::GetOperationalState() const
{
    if( humans_.empty() )
        return 0.;
    return (float)nNbrUsableHumans_ / (float)humans_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetNbrUsableHumans
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
inline
uint PHY_HumansComposante::GetNbrUsableHumans() const
{
    return nNbrUsableHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetComposante
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
inline
const PHY_ComposantePion& PHY_HumansComposante::GetComposante() const
{
    assert( pComposante_ );
    return *pComposante_;
}


