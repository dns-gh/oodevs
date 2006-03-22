// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.inl $
// $Author: Age $
// $Modtime: 4/05/05 14:44 $
// $Revision: 13 $
// $Workfile: PHY_ComposanteTypePion.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::FindComposanteType
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::FindComposanteType( const std::string& strName )
{
    CIT_ComposanteTypeMap it = composantesTypes_.find( strName );
    if( it == composantesTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::FindComposanteType
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::FindComposanteType( ASN1T_TypeEquipement nAsnID )
{
    for( CIT_ComposanteTypeMap it = composantesTypes_.begin(); it != composantesTypes_.end(); ++it )
    {
        const PHY_ComposanteTypePion& composanteType = *it->second;
        if( composanteType.GetMosID() == nAsnID )
            return &composanteType;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetDotationCapacities
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
inline
const PHY_DotationCapacities& PHY_ComposanteTypePion::GetDotationCapacities() const
{   
    return dotationCapacities_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumanInCrew
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteTypePion::GetNbrHumanInCrew() const
{
    return nNbrHumanInCrew_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetMaxSpeed( const TerrainData& data ) const
{
    return speeds_.GetMaxSpeed( data );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetMaxSpeed() const
{
    return speeds_.GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportHumans
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanTransportHumans() const
{
    return rNbrHumansLoadedPerTimeStep_ != 0. && rNbrHumansUnloadedPerTimeStep_ != 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedPerTimeStep
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansLoadedPerTimeStep() const
{
    return rNbrHumansLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep() const
{
    return rNbrHumansUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransport
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanTransport() const
{
    return rTransportWeightCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetTransportWeightCapacity
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetTransportWeightCapacity() const
{
    return rTransportWeightCapacity_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetTransportWeightLoadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetTransportWeightLoadedPerTimeStep() const
{
    return rTransportWeightLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetTransportWeightUnloadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetTransportWeightUnloadedPerTimeStep() const
{
    return rTransportWeightUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetSensorRotationAngle_
// Created: JVT 2004-10-20
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetSensorRotationAngle() const
{
    return rSensorRotationAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetWeight
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetWeight() const
{
    return rWeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetMaxSlope() const
{
    return rMaxSlope_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::operator==
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::operator==( const PHY_ComposanteTypePion& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::operator!=
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::operator!=( const PHY_ComposanteTypePion& rhs ) const
{
    return this != &rhs;
}

// =============================================================================
// LOGISTIC - MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
inline
PHY_ComposanteTypePion::sNTICapability::sNTICapability()
    : pMaintenanceLevel_( 0 )
    , bMobility_        ( false )
    , bElectronic_      ( false )
    , nMaxTime_         ( std::numeric_limits< uint >::max() )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
inline
PHY_ComposanteTypePion::sNTICapability::sNTICapability( const PHY_MaintenanceLevel& maintenanceLevel )
    : pMaintenanceLevel_( &maintenanceLevel )
    , bMobility_        ( false  )
    , bElectronic_      ( false )
    , nMaxTime_         ( std::numeric_limits< uint >::max() )
{

}
 
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sNTICapability::operator<
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::sNTICapability::operator < ( const sNTICapability& rhs ) const
{
    return pMaintenanceLevel_ < rhs.pMaintenanceLevel_;
}


// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanRepair
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanRepair() const
{
    return !ntiCapabilities_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanHaul
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanHaul() const
{
    return rHaulWeightLoadedPerTimeStep_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulWeightCapacity
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetHaulWeightCapacity() const
{
    return rHaulWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulWeightLoadedPerTimeStep
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetHaulWeightLoadedPerTimeStep() const
{
    return rHaulWeightLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulWeightUnloadedPerTimeStep
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetHaulWeightUnloadedPerTimeStep() const
{
    return rHaulWeightUnloadedPerTimeStep_;
}

// =============================================================================
// LOGISTIC - MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanDiagnoseHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanDiagnoseHumans() const
{
    return bCanDiagnoseHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanSortHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanSortHumans() const
{
    return bCanSortHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanEvacuateCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanEvacuateCasualties() const
{
    return nAmbulanceEvacuationCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanCollectCasualties
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanCollectCasualties() const
{
    return nAmbulanceCollectionCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAmbulanceCollectionCapacity
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteTypePion::GetAmbulanceCollectionCapacity() const
{
    return nAmbulanceCollectionCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep() const
{
    return rNbrHumansLoadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep() const
{
    return rNbrHumansUnloadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity() const
{
    return nAmbulanceEvacuationCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep() const
{
    return rNbrHumansLoadedForEvacuationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
MT_Float PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep() const
{
    return rNbrHumansUnloadedForEvacuationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanConvoyCommand
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanConvoyCommand() const
{
    return bConvoyCommander_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanConvoyTransport
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanConvoyTransport() const
{
    return rConvoyTransporterWeightCapacity_ > 0. && rConvoyTransporterVolumeCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConvoyTransporterLoadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteTypePion::GetConvoyTransporterLoadingTime() const
{
    return nConvoyTransporterLoadingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConvoyTransporterUnloadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteTypePion::GetConvoyTransporterUnloadingTime() const
{
    return nConvoyTransporterUnloadingTime_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetConvoyTransporterCapacity
// Created: NLD 2005-07-18
// -----------------------------------------------------------------------------
inline
void PHY_ComposanteTypePion::GetConvoyTransporterCapacity( MT_Float& rWeightMax, MT_Float& rVolumeMax ) const
{
    rWeightMax = rConvoyTransporterWeightCapacity_;
    rVolumeMax = rConvoyTransporterVolumeCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sBreakdownTypeProbability constructor
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
inline
PHY_ComposanteTypePion::sBreakdownTypeProbability::sBreakdownTypeProbability( const PHY_BreakdownType& breakdownType, MT_Float rProbabilityBound )
    : pBreakdownType_   ( &breakdownType )
    , rProbabilityBound_( rProbabilityBound ) 
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetRandomBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType& PHY_ComposanteTypePion::GetRandomBreakdownType() const
{
    return GetBreakdownType( randomBreakdownTypeProbabilities_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAttritionBreakdownType
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
inline
const PHY_BreakdownType& PHY_ComposanteTypePion::GetAttritionBreakdownType() const
{
    return GetBreakdownType( attritionBreakdownTypeProbabilities_ );
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::ApplyOnRadars
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
template< typename T > inline
void PHY_ComposanteTypePion::ApplyOnRadars( T& func ) const
{
    for( CIT_RadarTypeSet itRadar = radarTypes_.begin(); itRadar != radarTypes_.end(); ++itRadar )
        func( **itRadar );
}
