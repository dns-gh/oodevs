// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_BreakdownType.cpp $
// $Author: Jvt $
// $Modtime: 26/04/05 15:52 $
// $Revision: 7 $
// $Workfile: PHY_BreakdownType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_BreakdownType.h"
#include "PHY_MaintenanceLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Tools/MIL_Tools.h"
#include "Tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_BreakdownType::T_BreakdownMap PHY_BreakdownType::breakdowns_;
uint                              PHY_BreakdownType::nDiagnosticTime_ = 0;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

struct PHY_BreakdownType::LoadingWrapper
{
    void ReadCategory( xml::xistream& xis )
    {
        PHY_BreakdownType::ReadCategory( xis );
    }
    void ReadBreakdown( xml::xistream& xis, const PHY_MaintenanceLevel& maintenanceLevel )
    {
        PHY_BreakdownType::ReadBreakdown( xis, maintenanceLevel );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ConvertType
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
PHY_BreakdownType::E_Type PHY_BreakdownType::ConvertType( const std::string& strType )
{
    if( sCaseInsensitiveEqual()( strType, "M" ) )
        return eMobility;
    if( sCaseInsensitiveEqual()( strType, "EA" ) )
        return eElectronic;
    return eUnknown;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_BreakdownType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing breakdown types" );

    MT_Float rTimeVal;
    std::string timeVal;
    LoadingWrapper loader;
    xis >> start( "breakdowns" )
            >> start( "diagnosis" )
                >> attribute( "time", timeVal )
            >> end();
    if( ! tools::DecodeTime( timeVal, rTimeVal ) || rTimeVal < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "diagnosis: time < 0" );
    nDiagnosticTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );
    xis >> list( "category", loader, &LoadingWrapper::ReadCategory )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadCategory
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadCategory( xml::xistream& xis )
{
    const PHY_MaintenanceLevel::T_MaintenanceLevelMap& maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    std::string categoryType;
    xis >> attribute( "name", categoryType );

    PHY_MaintenanceLevel::CIT_MaintenanceLevelMap it = maintenanceLevels.find( categoryType );
    const PHY_MaintenanceLevel& maintenanceLevel = *it->second;

    LoadingWrapper loader;
    xis >> list( "breakdown", loader, &LoadingWrapper::ReadBreakdown, maintenanceLevel );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadBreakdown
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadBreakdown( xml::xistream& xis, const PHY_MaintenanceLevel& maintenanceLevel )
{
    std::set< uint > ids_;
    std::string strBreakdown;
    std::string strType;
    xis >> attribute( "name", strBreakdown )
        >> attribute( "type", strType );

    const E_Type nType = ConvertType( strType );
    if( nType == eUnknown )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unkown breakdown type" ); // $$$$ ABL 2007-07-20: error context

    const PHY_BreakdownType*& pBreakdown = breakdowns_[ strBreakdown ];
    if( pBreakdown )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Breakdown already exists" ); // $$$$ ABL 2007-07-20: error context

    pBreakdown = new PHY_BreakdownType( strBreakdown, maintenanceLevel, nType, xis );
    if( !ids_.insert( pBreakdown->GetID() ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Breakdown ID already used" ); // $$$$ ABL 2007-07-20: error context
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_BreakdownType::Terminate()
{
    for( CIT_BreakdownMap it = breakdowns_.begin(); it != breakdowns_.end(); ++it )
        delete it->second;
    breakdowns_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType constructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
PHY_BreakdownType::PHY_BreakdownType( const std::string& strName, const PHY_MaintenanceLevel& maintenanceLevel, E_Type nType, xml::xistream& xis )
    : strName_           ( strName )
    , maintenanceLevel_  ( maintenanceLevel )
    , nType_             ( nType )
    , nTheoricRepairTime_( 0 )
{
    std::string repairTime, variance;
    MT_Float rVariance;
    xis >> attribute( "id", nID_ )
        >> attribute( "average-repairing-time", repairTime )
        >> attribute( "variance", variance );

    if( !tools::DecodeTime( repairTime, nTheoricRepairTime_ ) || nTheoricRepairTime_ < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "average-repairing-time < 0" );
    nTheoricRepairTime_ = (uint)MIL_Tools::ConvertSecondsToSim( nTheoricRepairTime_ );
    if( !tools::DecodeTime( variance, rVariance ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "variance < 0" );
    rVariance = fabs( MIL_Tools::ConvertSecondsToSim( rVariance ) );

    repairTime_ = MT_GaussianRandom( nTheoricRepairTime_, rVariance );

    // Parts
    xis >> list( "part", *this, &PHY_BreakdownType::ReadPart );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadPart
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadPart( xml::xistream& xis )
{
    std::string strCategory;
    xis >> attribute( "dotation", strCategory );

    const PHY_DotationCategory* pCategory = PHY_DotationType::piece_.FindDotationCategory( strCategory );
    if( !pCategory )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown part category" ); // $$$$ ABL 2007-07-20: error context

    uint& nNbr = parts_[ pCategory ];
    if( nNbr > 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Part already initialized" ); // $$$$ ABL 2007-07-20: error context
    xis >> attribute( "quantity", nNbr );
    if( nNbr < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "part: quantity < 1" );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_BreakdownType::~PHY_BreakdownType()
{
}
