// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Logistic/PHY_LogWorkTime.cpp $
// $Author: Nld $
// $Modtime: 14/01/05 16:17 $
// $Revision: 2 $
// $Workfile: PHY_LogWorkTime.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_LogWorkTime.h"
#include "PHY_Breakdown.h"

PHY_LogWorkTime::T_WorkTimeMap PHY_LogWorkTime::workTimes_;

PHY_LogWorkTime PHY_LogWorkTime::time8Hours_ ( "Bordee8Heures" , EnumTempsBordee::temps_8_heures  );
PHY_LogWorkTime PHY_LogWorkTime::time12Hours_( "Bordee12Heures", EnumTempsBordee::temps_12_heures );
PHY_LogWorkTime PHY_LogWorkTime::time16Hours_( "Bordee16Heures", EnumTempsBordee::temps_16_heures );
PHY_LogWorkTime PHY_LogWorkTime::time20Hours_( "Bordee20Heures", EnumTempsBordee::temps_20_heures );
PHY_LogWorkTime PHY_LogWorkTime::time24Hours_( "Bordee24Heures", EnumTempsBordee::temps_24_heures );

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::Initialize
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_LogWorkTime::Initialize( MIL_InputArchive& archive )
{
    workTimes_[ time8Hours_ .GetName() ] = &time8Hours_;
    workTimes_[ time12Hours_.GetName() ] = &time12Hours_;
    workTimes_[ time16Hours_.GetName() ] = &time16Hours_;
    workTimes_[ time20Hours_.GetName() ] = &time20Hours_;
    workTimes_[ time24Hours_.GetName() ] = &time24Hours_;

    archive.Section( "Logistique" );
    archive.Section( "TempsDeBordee" );

    for( CIT_WorkTimeMap it = workTimes_.begin(); it != workTimes_.end(); ++it )
    {
        PHY_LogWorkTime& workTime = const_cast< PHY_LogWorkTime& >( *it->second );
        archive.Section( workTime.GetName() );

        workTime.ReadData( archive );

        archive.EndSection();
    }

    archive.EndSection(); // TempsDeBordee
    archive.EndSection(); // Logistique
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::Terminate
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
void PHY_LogWorkTime::Terminate()
{
    workTimes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime constructor
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
PHY_LogWorkTime::PHY_LogWorkTime( const std::string& strName, ASN1T_EnumTempsBordee asn )
    : strName_           ( strName )
    , asn_               ( asn )
    , rRepairTimeFactor_ ( 1. )
    , rSortingTimeFactor_( 1. )
    , rHealingTimeFactor_( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime destructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_LogWorkTime::~PHY_LogWorkTime()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_LogWorkTime::ReadData
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_LogWorkTime::ReadData( MIL_InputArchive& archive )
{
    archive.ReadField( "CoefTempsReparation", rRepairTimeFactor_ , CheckValueGreater( 0. ) );
    archive.ReadField( "CoefTempsSoin"      , rHealingTimeFactor_, CheckValueGreater( 0. ) );
    archive.ReadField( "CoefTempsTri"       , rSortingTimeFactor_, CheckValueGreater( 0. ) );
}
