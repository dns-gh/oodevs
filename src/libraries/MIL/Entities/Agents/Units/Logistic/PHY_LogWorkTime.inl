// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_LogWorkTime.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 14:26 $
// $Revision: 4 $
// $Workfile: PHY_LogWorkTime.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetAsnID
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
ASN1T_EnumTempsBordee PHY_LogWorkTime::GetAsnID() const
{
    return asn_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetRepairTimeFactor
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_LogWorkTime::GetRepairTimeFactor() const
{
    return rRepairTimeFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetSortingTimeFactor
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
inline
MT_Float PHY_LogWorkTime::GetSortingTimeFactor() const
{
    return rSortingTimeFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetHealingTimeFactor
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
inline
MT_Float PHY_LogWorkTime::GetHealingTimeFactor() const
{
    return rHealingTimeFactor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetName
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
const std::string& PHY_LogWorkTime::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::Find
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
const PHY_LogWorkTime* PHY_LogWorkTime::Find( ASN1T_EnumTempsBordee nID )
{
    for( CIT_WorkTimeMap it = workTimes_.begin(); it != workTimes_.end(); ++it )
    {
        if( it->second->GetAsnID() == nID )
            return it->second;
    }
    return 0;
}
