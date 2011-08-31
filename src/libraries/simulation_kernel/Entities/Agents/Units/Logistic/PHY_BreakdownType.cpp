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
#include "tools/xmlcodecs.h"
#include "MIL_Random.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"
#include <xeumeuleu/xml.hpp>

PHY_BreakdownType::T_BreakdownMap PHY_BreakdownType::breakdowns_;
unsigned int                      PHY_BreakdownType::nDiagnosticTime_ = 0;

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
    double rTimeVal;
    std::string timeVal;
    LoadingWrapper loader;
    xis >> xml::start( "breakdowns" )
            >> xml::start( "diagnosis" )
                >> xml::attribute( "time", timeVal )
            >> xml::end;
    if( ! tools::DecodeTime( timeVal, rTimeVal ) || rTimeVal < 0 )
        xis.error( "diagnosis: time < 0" );
    nDiagnosticTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( rTimeVal );
    xis >> xml::list( "category", loader, &LoadingWrapper::ReadCategory )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadCategory
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadCategory( xml::xistream& xis )
{
    const PHY_MaintenanceLevel::T_MaintenanceLevelMap& maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    std::string categoryType;
    xis >> xml::attribute( "name", categoryType );

    PHY_MaintenanceLevel::CIT_MaintenanceLevelMap it = maintenanceLevels.find( categoryType );
    const PHY_MaintenanceLevel& maintenanceLevel = *it->second;

    LoadingWrapper loader;
    xis >> xml::list( "breakdown", loader, &LoadingWrapper::ReadBreakdown, maintenanceLevel );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadBreakdown
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadBreakdown( xml::xistream& xis, const PHY_MaintenanceLevel& maintenanceLevel )
{
    std::set< unsigned int > ids_;
    std::string strBreakdown;
    std::string strType;
    xis >> xml::attribute( "name", strBreakdown )
        >> xml::attribute( "type", strType );

    const E_Type nType = ConvertType( strType );
    if( nType == eUnknown )
        xis.error( "Unkown breakdown type" );

    const PHY_BreakdownType*& pBreakdown = breakdowns_[ strBreakdown ];
    if( pBreakdown )
        xis.error( "Breakdown already exists" );

    pBreakdown = new PHY_BreakdownType( strBreakdown, maintenanceLevel, nType, xis );
    if( !ids_.insert( pBreakdown->GetID() ).second )
        xis.error( "Breakdown ID already used" );
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
    double rVariance;
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "average-repairing-time", repairTime )
        >> xml::attribute( "variance", variance );

    if( !tools::DecodeTime( repairTime, nTheoricRepairTime_ ) || nTheoricRepairTime_ < 0 )
        xis.error( "average-repairing-time < 0" );
    nTheoricRepairTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( nTheoricRepairTime_ );
    if( !tools::DecodeTime( variance, rVariance ) )
        xis.error( "variance < 0" );
    rVariance = fabs( MIL_Tools::ConvertSecondsToSim( rVariance ) );

    repairTime_ = MT_GaussianRandom( nTheoricRepairTime_, rVariance );

    // Parts
    xis >> xml::list( "part", *this, &PHY_BreakdownType::ReadPart );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadPart
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadPart( xml::xistream& xis )
{
    std::string strCategory;
    xis >> xml::attribute( "resource", strCategory );

    const PHY_DotationCategory* pCategory = PHY_DotationType::piece_->FindDotationCategory( strCategory );
    if( !pCategory )
        xis.error( "Unknown part category" );

    unsigned int& nNbr = parts_[ pCategory ];
    if( nNbr > 0 )
        xis.error( "Part already initialized" );
    xis >> xml::attribute( "quantity", nNbr );
    if( nNbr < 1 )
        xis.error( "part: quantity < 1" );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_BreakdownType::~PHY_BreakdownType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetDiagnosticTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
unsigned int PHY_BreakdownType::GetDiagnosticTime()
{
    return nDiagnosticTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_BreakdownType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetMosID
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
unsigned int PHY_BreakdownType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Find
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
const PHY_BreakdownType* PHY_BreakdownType::Find( const std::string& strName )
{
    CIT_BreakdownMap it = breakdowns_.find( strName );

    return it == breakdowns_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_BreakdownType* PHY_BreakdownType::Find( unsigned int nID )
{
    CIT_BreakdownMap it = std::find_if( breakdowns_.begin(), breakdowns_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_BreakdownType::GetID ), std::select2nd< T_BreakdownMap::value_type >() ) ) );

    return it == breakdowns_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::AffectMobility
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_BreakdownType::AffectMobility() const
{
    return nType_ == eMobility;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::AffectElectronic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_BreakdownType::AffectElectronic() const
{
    return nType_ == eElectronic;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetMaintenanceLevel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_MaintenanceLevel& PHY_BreakdownType::GetMaintenanceLevel() const
{
    return maintenanceLevel_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetParts
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_BreakdownType::T_PartMap& PHY_BreakdownType::GetParts() const
{
    return parts_;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ChooseARepairTime
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
unsigned int PHY_BreakdownType::ChooseARepairTime() const
{
    return (unsigned int)repairTime_.rand();
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::GetTheoricRepairTime
// Created: NLD 2006-06-01
// -----------------------------------------------------------------------------
unsigned int PHY_BreakdownType::GetTheoricRepairTime() const
{
    return nTheoricRepairTime_;
}
