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
#include "tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_MaintenanceWorkRate::T_WorkRateMap PHY_MaintenanceWorkRate::workRates_;

PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r1_( "Regime1", EnumLogMaintenanceRegimeTravail::regime_1 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r2_( "Regime2", EnumLogMaintenanceRegimeTravail::regime_2 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r3_( "Regime3", EnumLogMaintenanceRegimeTravail::regime_3 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r4_( "Regime4", EnumLogMaintenanceRegimeTravail::regime_4 );

struct PHY_MaintenanceWorkRate::LoadingWrapper
{
    void ReadWorkRate( xml::xistream& xis )
    {
        PHY_MaintenanceWorkRate::ReadWorkRate( xis );
    }
};

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

    LoadingWrapper loader;
    xis >> start( "maintenance" )
            >> start( "working-schemes" )
                >> xml::list( "working-scheme", loader, &LoadingWrapper::ReadWorkRate )
            >> end()
        >> end();
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
PHY_MaintenanceWorkRate::PHY_MaintenanceWorkRate( const std::string& strName, ASN1T_EnumLogMaintenanceRegimeTravail asn )
    : strName_              ( strName )
    , asn_                  ( asn )
    , rWorkerRatio_         ( 1. )
    , rWorkTime_            ( 0. )
    , nDelayBeforeWarningRC_( std::numeric_limits< uint >::max() )
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
    xis >> attribute( "type", name );

    T_WorkRateMap::iterator it = workRates_.find( name );
    if( it == workRates_.end() )
        xis.error( "Undefined working scheme" );

    PHY_MaintenanceWorkRate& workRate = const_cast< PHY_MaintenanceWorkRate& >( *it->second );
    xis >> attribute( "working-time", workRate.rWorkTime_ );
    if( workRate.rWorkTime_ <= 0 )
        xis.error( "Working time <= 0" );
    workRate.rWorkerRatio_ = workRate.rWorkTime_ / 24.;

    std::string time;
    xis >> optional() >> attribute( "time-before-warning", time );
    if( tools::DecodeTime( time, workRate.nDelayBeforeWarningRC_ ) && workRate.nDelayBeforeWarningRC_ == 0 )
        xis.error( "Time before warning is null" );
}
