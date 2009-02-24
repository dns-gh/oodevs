// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/MIL_PopulationType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:15 $
// $Revision: 2 $
// $Workfile: MIL_PopulationType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DIA_FunctionTable< MIL_Population >& MIL_PopulationType::GetFunctionTable
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
DIA_FunctionTable< MIL_Population >& MIL_PopulationType::GetFunctionTable() const
{
    assert( pDIAFunctionTable_ );
    return *pDIAFunctionTable_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetModel
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC& MIL_PopulationType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetConcentrationDensity
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationType::GetConcentrationDensity() const
{
    return rConcentrationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetDefaultFlowDensity
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationType::GetDefaultFlowDensity() const
{
    return rDefaultFlowDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PopulationType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMosID
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationType::GetMaxSpeed() const
{
    return rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetAttritionData
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
inline
const PHY_AttritionData& MIL_PopulationType::GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection ) const
{
    return attritionData_.GetAttritionData( attitude, protection );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::GetPH
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationType::GetPH( const MIL_PopulationAttitude& attitude, MT_Float rDensity ) const
{
    return attritionData_.GetPH( attitude, rDensity );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Find
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
const MIL_PopulationType* MIL_PopulationType::Find( const std::string& strName )
{
    CIT_PopulationMap it = populations_.find( strName );   
    return it == populations_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Find
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
inline
const MIL_PopulationType* MIL_PopulationType::Find( uint nID )
{
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        if( it->second->nID_ == nID )
            return it->second;
    return 0;
}

