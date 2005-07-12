// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AutomateLOG.inl $
// $Author: Nld $
// $Modtime: 2/02/05 16:37 $
// $Revision: 3 $
// $Workfile: MIL_AutomateLOG.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetMaintenanceSuperior
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_AutomateLOG::GetMaintenanceSuperior() const
{
    return pMaintenanceSuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetMedicalSuperior
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_AutomateLOG::GetMedicalSuperior() const
{
    return pMedicalSuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetSupplySuperior
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_AutomateLOG::GetSupplySuperior() const
{
    return pSupplySuperior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateLOG::GetQuota
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
inline
MT_Float MIL_AutomateLOG::GetQuota( const PHY_DotationCategory& dotationCategory ) const
{
    CIT_DotationQuotaMap it = stockQuotas_.find( &dotationCategory );
    if( it == stockQuotas_.end() )
        return 0.;
    return it->second.rQuota_;
}
