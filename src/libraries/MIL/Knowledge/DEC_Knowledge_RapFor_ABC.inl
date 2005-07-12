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


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapFor_ABC::GetValue
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_RapFor_ABC::GetValue() const
{
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
