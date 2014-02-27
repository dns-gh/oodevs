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
#include "tools/Codec.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

PHY_BreakdownType::T_BreakdownMap PHY_BreakdownType::breakdowns_;
unsigned int                      PHY_BreakdownType::nDiagnosticTime_ = 0;

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ConvertType
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
inline
PHY_BreakdownType::E_Type PHY_BreakdownType::ConvertType( const std::string& strType )
{
    if( boost::iequals( strType, "M" ) )
        return eMobility;
    if( boost::iequals( strType, "EA" ) )
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
    xis >> xml::start( "breakdowns" )
            >> xml::start( "diagnosis" )
                >> xml::attribute( "time", timeVal )
            >> xml::end;
    if( ! tools::DecodeTime( timeVal, rTimeVal ) || rTimeVal < 0 )
        throw MASA_EXCEPTION( xis.context() + "diagnosis: time < 0" );
    nDiagnosticTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( rTimeVal );
    xis >> xml::list( "category", &PHY_BreakdownType::ReadCategory )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::ReadCategory
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_BreakdownType::ReadCategory( xml::xistream& xis )
{
    const auto maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    const std::string categoryType = xis.attribute< std::string >( "name" );
    const PHY_MaintenanceLevel& maintenanceLevel = *maintenanceLevels.find( categoryType )->second;
    xis >> xml::list( "breakdown", boost::bind( &PHY_BreakdownType::ReadBreakdown, _1, boost::cref( maintenanceLevel ) ) );
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
        throw MASA_EXCEPTION( xis.context() + "Unkown breakdown type" );

    const PHY_BreakdownType*& pBreakdown = breakdowns_[ strBreakdown ];
    if( pBreakdown )
        throw MASA_EXCEPTION( xis.context() + "Breakdown already exists" );

    pBreakdown = new PHY_BreakdownType( strBreakdown, maintenanceLevel, nType, xis );
    if( !ids_.insert( pBreakdown->GetID() ).second )
        throw MASA_EXCEPTION( xis.context() + "Breakdown ID already used" );
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_BreakdownType::Terminate()
{
    for( auto it = breakdowns_.begin(); it != breakdowns_.end(); ++it )
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
        throw MASA_EXCEPTION( xis.context() + "average-repairing-time < 0" );
    nTheoricRepairTime_ = (unsigned int)MIL_Tools::ConvertSecondsToSim( nTheoricRepairTime_ );
    if( !tools::DecodeTime( variance, rVariance ) )
        throw MASA_EXCEPTION( xis.context() + "variance < 0" );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown part category" );

    unsigned int& nNbr = parts_[ pCategory ];
    if( nNbr > 0 )
        throw MASA_EXCEPTION( xis.context() + "Part already initialized" );
    xis >> xml::attribute( "quantity", nNbr );
    if( nNbr < 1 )
        throw MASA_EXCEPTION( xis.context() + "part: quantity < 1" );
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
    for( auto it = breakdowns_.begin(); it != breakdowns_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
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
