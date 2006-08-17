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

PHY_BreakdownType::T_BreakdownMap PHY_BreakdownType::breakdowns_;
uint                              PHY_BreakdownType::nDiagnosticTime_ = 0;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

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
void PHY_BreakdownType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing breakdown types" );

    archive.Section( "Pannes" );
    MT_Float rTimeVal;
    archive.ReadTimeField( "TempsDiagnostique", rTimeVal, CheckValueGreaterOrEqual( 0. ) );
    nDiagnosticTime_ = (uint)MIL_Tools::ConvertSecondsToSim( rTimeVal );

    archive.Section( "Types" );
    std::set< uint > ids_;
    const PHY_MaintenanceLevel::T_MaintenanceLevelMap& maintenanceLevels = PHY_MaintenanceLevel::GetMaintenanceLevels();
    for( PHY_MaintenanceLevel::CIT_MaintenanceLevelMap it = maintenanceLevels.begin(); it != maintenanceLevels.end(); ++it )
    {
        const PHY_MaintenanceLevel& maintenanceLevel = *it->second;
        archive.BeginList( maintenanceLevel.GetName() );

        while( archive.NextListElement() )
        {
            archive.Section( "Panne" );
            std::string strBreakdown;
            std::string strType;
            archive.ReadAttribute( "nom" , strBreakdown );
            archive.ReadAttribute( "type", strType );

            const E_Type nType = ConvertType( strType );
            if( nType == eUnknown )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unkown breakdown type", archive.GetContext() );

            const PHY_BreakdownType*& pBreakdown = breakdowns_[ strBreakdown ];
            if( pBreakdown )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Breakdown already exists", archive.GetContext() );

            pBreakdown = new PHY_BreakdownType( strBreakdown, maintenanceLevel, nType, archive );       
            if( !ids_.insert( pBreakdown->GetID() ).second )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Breakdown ID already used", archive.GetContext() );
            
            archive.EndSection(); // Panne
        }
        archive.EndList(); // NTIx
    }    
    archive.EndSection(); // Types 
    archive.EndSection(); // Pannes
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
PHY_BreakdownType::PHY_BreakdownType( const std::string& strName, const PHY_MaintenanceLevel& maintenanceLevel, E_Type nType, MIL_InputArchive& archive )
    : strName_           ( strName )
    , maintenanceLevel_  ( maintenanceLevel )
    , nType_             ( nType )
    , nTheoricRepairTime_( 0 )
{
    archive.ReadField( "MosID", nID_ );

    archive.Section( "Reparation" );

    archive.ReadTimeAttribute( "tempsMoyen", nTheoricRepairTime_, CheckValueGreaterOrEqual( 0 ) );
    nTheoricRepairTime_ = (uint)MIL_Tools::ConvertSecondsToSim( nTheoricRepairTime_ );
    
    MT_Float rVariance;   
    archive.ReadTimeAttribute( "variance", rVariance );
    rVariance = fabs( MIL_Tools::ConvertSecondsToSim( rVariance ) );

    repairTime_ = MT_GaussianRandom( nTheoricRepairTime_, rVariance );
    archive.EndSection(); // Reparation

    // Parts
    archive.BeginList( "Pieces" );
    while( archive.NextListElement() )
    {
        archive.Section( "Piece" );
        std::string strCategory;
        archive.ReadAttribute( "categorie", strCategory );
        
        const PHY_DotationCategory* pCategory = PHY_DotationType::piece_.FindDotationCategory( strCategory );
        if( !pCategory )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown part category", archive.GetContext() );

        uint& nNbr = parts_[ pCategory ];
        if( nNbr > 0 )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Part already initialized", archive.GetContext() );
        archive.Read( nNbr, CheckValueGreaterOrEqual( 1 ) );

        archive.EndSection(); // Piece
    }
    archive.EndList(); // Pieces
}

// -----------------------------------------------------------------------------
// Name: PHY_BreakdownType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_BreakdownType::~PHY_BreakdownType()
{
}

