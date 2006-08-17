// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 14:26 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceWorkRate.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::Find
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
const PHY_MaintenanceWorkRate* PHY_MaintenanceWorkRate::Find( ASN1T_EnumLogMaintenanceRegimeTravail nID )
{
    for( CIT_WorkRateMap it = workRates_.begin(); it != workRates_.end(); ++it )
    {
        if( it->second->GetAsnID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetAsnID
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
ASN1T_EnumLogMaintenanceRegimeTravail PHY_MaintenanceWorkRate::GetAsnID() const
{
    return asn_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetName
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
const std::string& PHY_MaintenanceWorkRate::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetNbrWorkerAllowedToWork
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
inline
uint PHY_MaintenanceWorkRate::GetNbrWorkerAllowedToWork( uint nNbrAvailable ) const
{
    return std::min( nNbrAvailable, (uint)ceil( rWorkerRatio_ * nNbrAvailable ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetDelayBeforeWarningRC
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
inline
uint PHY_MaintenanceWorkRate::GetDelayBeforeWarningRC() const
{
    return nDelayBeforeWarningRC_;
}
