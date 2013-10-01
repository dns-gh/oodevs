// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.cpp $
// $Author: Nld $
// $Modtime: 14/01/05 16:17 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceWorkRate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceWorkRate.h"
#include "PHY_Breakdown.h"
#include "protocol/Protocol.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

PHY_MaintenanceWorkRate::T_WorkRateMap PHY_MaintenanceWorkRate::workRates_;

PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r1_( "Regime1", sword::rate_1 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r2_( "Regime2", sword::rate_2 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r3_( "Regime3", sword::rate_3 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r4_( "Regime4", sword::rate_4 );

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::Initialize
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_MaintenanceWorkRate::Initialize( xml::xistream& xis )
{
    workRates_[ r1_.GetName() ] = &r1_;
    workRates_[ r2_.GetName() ] = &r2_;
    workRates_[ r3_.GetName() ] = &r3_;
    workRates_[ r4_.GetName() ] = &r4_;
    xis >> xml::start( "maintenance" )
            >> xml::start( "working-schemes" )
                >> xml::list( "working-scheme", &PHY_MaintenanceWorkRate::ReadWorkRate )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::Terminate
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_MaintenanceWorkRate::Terminate()
{
    workRates_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate constructor
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
PHY_MaintenanceWorkRate::PHY_MaintenanceWorkRate( const std::string& strName, sword::EnumLogMaintenanceWorkRate asn )
    : strName_              ( strName )
    , asn_                  ( asn )
    , rWorkerRatio_         ( 1. )
    , rWorkTime_            ( 0. )
    , nDelayBeforeWarningRC_( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate destructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_MaintenanceWorkRate::~PHY_MaintenanceWorkRate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::ReadWorkRate
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_MaintenanceWorkRate::ReadWorkRate( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "type", name );

    T_WorkRateMap::iterator it = workRates_.find( name );
    if( it == workRates_.end() )
        throw MASA_EXCEPTION( xis.context() + "Undefined working scheme" );

    PHY_MaintenanceWorkRate& workRate = const_cast< PHY_MaintenanceWorkRate& >( *it->second );
    xis >> xml::attribute( "working-time", workRate.rWorkTime_ );
    if( workRate.rWorkTime_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "Working time <= 0" );
    workRate.rWorkerRatio_ = workRate.rWorkTime_ / 24.;

    std::string time;
    xis >> xml::optional >> xml::attribute( "time-before-warning", time );
    if( tools::DecodeTime( time, workRate.nDelayBeforeWarningRC_ ) && workRate.nDelayBeforeWarningRC_ == 0 )
        throw MASA_EXCEPTION( xis.context() + "Time before warning is null" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::Find
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
const PHY_MaintenanceWorkRate* PHY_MaintenanceWorkRate::Find( sword::EnumLogMaintenanceWorkRate nID )
{
    for( auto it = workRates_.begin(); it != workRates_.end(); ++it )
        if( it->second->GetAsnID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetAsnID
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
sword::EnumLogMaintenanceWorkRate PHY_MaintenanceWorkRate::GetAsnID() const
{
    return asn_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetName
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
const std::string& PHY_MaintenanceWorkRate::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetNbrWorkerAllowedToWork
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
unsigned int PHY_MaintenanceWorkRate::GetNbrWorkerAllowedToWork( unsigned int nNbrAvailable ) const
{
    return std::min( nNbrAvailable, (unsigned int)ceil( rWorkerRatio_ * nNbrAvailable ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::GetDelayBeforeWarningRC
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
unsigned int PHY_MaintenanceWorkRate::GetDelayBeforeWarningRC() const
{
    return nDelayBeforeWarningRC_;
}
