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
// Name: PHY_ComposanteTypePion::Find
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::Find( const std::string& strName )
{
    CIT_ComposanteTypeMap it = composantesTypes_.find( strName );
    if( it == composantesTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::Find
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteTypePion* PHY_ComposanteTypePion::Find( sword::EquipmentType nAsnID )
{
    for( CIT_ComposanteTypeMap it = composantesTypes_.begin(); it != composantesTypes_.end(); ++it )
    {
        const PHY_ComposanteTypePion& composanteType = *it->second;
        if( composanteType.GetMosID().id() == nAsnID.id() )
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
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetMaxSpeed( const TerrainData& data ) const
{
    return speeds_.GetMaxSpeed( data );
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetMaxSpeed() const
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
double PHY_ComposanteTypePion::GetNbrHumansLoadedPerTimeStep() const
{
    return rNbrHumansLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetNbrHumansUnloadedPerTimeStep() const
{
    return rNbrHumansUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportPion
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanTransportPion() const
{
    return rPionTransporterWeightCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightCapacity
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetPionTransporterWeightCapacity() const
{
    return rPionTransporterWeightCapacity_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightLoadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetPionTransporterWeightLoadedPerTimeStep() const
{
    return rPionTransporterWeightLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetPionTransporterWeightUnloadedPerTimeStep
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetPionTransporterWeightUnloadedPerTimeStep() const
{
    return rPionTransporterWeightUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::CanTransportCrowd
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteTypePion::CanTransportCrowd() const
{
    return nCrowdTransporterCapacity_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterCapacity
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
inline
unsigned int PHY_ComposanteTypePion::GetCrowdTransporterCapacity() const
{
    return nCrowdTransporterCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterLoadedPerTimeStep
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetCrowdTransporterLoadedPerTimeStep() const
{
    return rCrowdTransporterLoadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetCrowdTransporterUnloadedPerTimeStep
// Created: JSR 2011-08-08
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetCrowdTransporterUnloadedPerTimeStep() const
{
    return rCrowdTransporterUnloadedPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetSensorRotationAngle_
// Created: JVT 2004-10-20
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetSensorRotationAngle() const
{
    return rSensorRotationAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetWeight
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetWeight() const
{
    return rWeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetMaxSlope() const
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
    , nMaxTime_         ( std::numeric_limits< unsigned int >::max() )
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
    , nMaxTime_         ( std::numeric_limits< unsigned int >::max() )
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
    return rHaulerWeightCapacity_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerWeightCapacity
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetHaulerWeightCapacity() const
{
    return rHaulerWeightCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerLoadingTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetHaulerLoadingTime() const
{
    return rHaulerLoadingTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetHaulerUnloadingTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetHaulerUnloadingTime() const
{
    return rHaulerUnloadingTime_;
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
unsigned int PHY_ComposanteTypePion::GetAmbulanceCollectionCapacity() const
{
    return nAmbulanceCollectionCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetNbrHumansLoadedForCollectionPerTimeStep() const
{
    return rNbrHumansLoadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetNbrHumansUnloadedForCollectionPerTimeStep() const
{
    return rNbrHumansUnloadedForCollectionPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
unsigned int PHY_ComposanteTypePion::GetAmbulanceEvacuationCapacity() const
{
    return nAmbulanceEvacuationCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetNbrHumansLoadedForEvacuationPerTimeStep() const
{
    return rNbrHumansLoadedForEvacuationPerTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
double PHY_ComposanteTypePion::GetNbrHumansUnloadedForEvacuationPerTimeStep() const
{
    return rNbrHumansUnloadedForEvacuationPerTimeStep_;
}
   
// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetStockTransporterCapacity
// Created: NLD 2005-07-18
// -----------------------------------------------------------------------------
inline
void PHY_ComposanteTypePion::GetStockTransporterCapacity( double& rWeightMax, double& rVolumeMax ) const
{
    rWeightMax = rStockTransporterWeightCapacity_;
    rVolumeMax = rStockTransporterVolumeCapacity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::GetStockTransporterNature
// Created: NLD 2006-03-27
// -----------------------------------------------------------------------------
inline
const PHY_DotationNature* PHY_ComposanteTypePion::GetStockTransporterNature() const
{
    return pStockTransporterNature_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteTypePion::sBreakdownTypeProbability constructor
// Created: JVT 2005-04-26
// -----------------------------------------------------------------------------
inline
PHY_ComposanteTypePion::sBreakdownTypeProbability::sBreakdownTypeProbability( const PHY_BreakdownType& breakdownType, double rProbabilityBound )
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
