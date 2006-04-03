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

#include "MIL_pch.h"

#include "PHY_MaintenanceWorkRate.h"
#include "PHY_Breakdown.h"
#include "Tools/MIL_Tools.h"

PHY_MaintenanceWorkRate::T_WorkRateMap PHY_MaintenanceWorkRate::workRates_;

PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r0_( "Regime0", EnumLogMaintenanceRegimeTravail::regime_0 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r1_( "Regime1", EnumLogMaintenanceRegimeTravail::regime_1 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r2_( "Regime2", EnumLogMaintenanceRegimeTravail::regime_2 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r3_( "Regime3", EnumLogMaintenanceRegimeTravail::regime_3 );
PHY_MaintenanceWorkRate PHY_MaintenanceWorkRate::r4_( "Regime4", EnumLogMaintenanceRegimeTravail::regime_4 );

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::Initialize
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_MaintenanceWorkRate::Initialize( MIL_InputArchive& archive )
{
    workRates_[ r0_.GetName() ] = &r0_;
    workRates_[ r1_.GetName() ] = &r1_;
    workRates_[ r2_.GetName() ] = &r2_;
    workRates_[ r3_.GetName() ] = &r3_;
    workRates_[ r4_.GetName() ] = &r4_;

    archive.Section( "Maintenance" );
    archive.Section( "RegimesTravail" );

    for( CIT_WorkRateMap it = workRates_.begin(); it != workRates_.end(); ++it )
    {
        PHY_MaintenanceWorkRate& workRate = const_cast< PHY_MaintenanceWorkRate& >( *it->second );
        archive.Section( workRate.GetName() );

        workRate.ReadData( archive );

        archive.EndSection();
    }

    archive.EndSection(); // RegimesTravail
    archive.EndSection(); // Maintenance
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
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate destructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_MaintenanceWorkRate::~PHY_MaintenanceWorkRate()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceWorkRate::ReadData
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_MaintenanceWorkRate::ReadData( MIL_InputArchive& archive )
{
    archive.ReadAttribute( "dureeTravail", rWorkTime_, CheckValueGreater( 0. ) );
    rWorkerRatio_ = rWorkTime_ / 24.;

    if( archive.ReadTimeAttribute( "delaiAvantAvertissement", nDelayBeforeWarningRC_, CheckValueGreater( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        nDelayBeforeWarningRC_ = (uint)MIL_Tools::ConvertSecondsToSim( nDelayBeforeWarningRC_ );
}
