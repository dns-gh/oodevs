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

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetNbrWorkerAllowedToWork
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
inline
uint PHY_LogWorkTime::GetNbrWorkerAllowedToWork( uint nNbrAvailable ) const
{
    return (uint)ceil( rWorkerRatio_ * nNbrAvailable );
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::GetDelayBeforeWarningRC
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
inline
uint PHY_LogWorkTime::GetDelayBeforeWarningRC() const
{
    return nDelayBeforeWarningRC_;
}
