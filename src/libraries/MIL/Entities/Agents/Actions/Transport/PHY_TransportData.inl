// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Transport/PHY_TransportData.inl $
// $Author: Nld $
// $Modtime: 22/11/04 17:48 $
// $Revision: 1 $
// $Workfile: PHY_TransportData.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::GetWeightToTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
MT_Float PHY_TransportData::GetWeightToTransport() const
{
    MT_Float rWeight = 0;
    for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it )
        rWeight += it->second.rRemainingWeight_;
    return rWeight;
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::SetWeightToLoad
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
void PHY_TransportData::SetWeightToLoad( MT_Float rWeight )
{
    assert( rWeightToLoad_ == 0. );
    rWeightToLoad_ = rWeight;
}
