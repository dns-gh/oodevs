// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapFor_ABC.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 11:26 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_RapFor_ABC.inl $
//
// *****************************************************************************

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void DEC_Knowledge_RapFor_ABC::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template < typename Archive > 
void DEC_Knowledge_RapFor_ABC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & nLastCacheUpdateTick_
         & rRapForValue_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::GetValue
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_RapFor_ABC::GetValue()
{
    Update();
    return rRapForValue_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue()
{
    return rRapForIncreasePerTimeStepDefaultValue_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue( MT_Float rBaseTimeValue )
{
    if( rBaseTimeValue <= 0. )
        return rRapForBoundMax_ - rRapForBoundMin_;
    else
        return ( rRapForBoundMax_ - rRapForBoundMin_ ) / rBaseTimeValue;
}