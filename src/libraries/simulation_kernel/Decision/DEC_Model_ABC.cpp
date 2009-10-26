// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.cpp $
// $Author: Nld $
// $Modtime: 11/04/05 14:55 $
// $Revision: 9 $
// $Workfile: DEC_Model_ABC.cpp $
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_Model_ABC.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/Orders/MIL_FragOrderType.h"

#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_Model_ABC::DEC_Model_ABC( const std::string& strModel, xml::xistream& xis, const std::string& strSourcePath, const std::string& strPrefix, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
    : strModel_            ( strModel )
    , strIncludePath_      ( strSourcePath )
{
    xis >> xml::attribute( "dia-type", strDIAType_ )
        >> xml::attribute( "file", strScript_ );

    InitializeMissions( xis, missionTypes );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC destructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_Model_ABC::~DEC_Model_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::FileChanged
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::FileChanged( const std::string& strFileName, time_t since )
{
    static std::map< std::string, time_t > changedFiles; // $$$$ AGE 2005-03-22: ram ?

    std::map< std::string, time_t >::iterator itFile = changedFiles.find( strFileName );
    if( itFile == changedFiles.end() )
    {
        struct _stat scriptInfo;
        if( _stat( strFileName.c_str(), & scriptInfo ) == -1 )
            return true;
        itFile = changedFiles.insert( std::make_pair( strFileName, scriptInfo.st_mtime ) ).first;
    }
    return itFile->second > since;
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::InitializeMissions
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_Model_ABC::InitializeMissions( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
{
    availableMissions_.clear();
    availableFragOrdersPerMission_.clear();

    xis >> xml::optional() >> xml::start( "missions" )
                           >> xml::list( "mission", *this, &DEC_Model_ABC::ReadMission, missionTypes )
                      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void DEC_Model_ABC::ReadMission( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
{
    std::string strMission;
    xis >> xml::attribute( "name", strMission );

    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >::const_iterator it = missionTypes.find( strMission );
    if ( it == missionTypes.end() )
        xis.error( "Invalid mission name '" + strMission + "' for model '" + GetName() + "'" );
    const MIL_MissionType_ABC* pType = it->second;
    if( !pType )
        xis.error( "Invalid mission name '" + strMission + "' for model '" + GetName() + "'" );

	availableMissions_.insert( pType ) ;

    // Check if the DIA behavior is present
    if( !IsMissionAvailable( *pType ) )
        xis.error( "Missing behaviors for mission" );

    InitializeMissionFragOrders( xis, *pType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::InitializeMissionFragOrders
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_Model_ABC::InitializeMissionFragOrders( xml::xistream& xis, const MIL_MissionType_ABC& missionType )
{
    xis >> xml::list( "fragorder", *this, &DEC_Model_ABC::ReadFragOrder, missionType );
}
// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadFragOrder
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void DEC_Model_ABC::ReadFragOrder( xml::xistream& xis, const MIL_MissionType_ABC& missionType )
{
	std::string strOrdreConduite;
	xis >> xml::attribute( "name", strOrdreConduite );

	const MIL_FragOrderType* pType = MIL_FragOrderType::Find( strOrdreConduite );
	if( !pType )
		xis.error( "Unknown orderConduite type" );

	availableFragOrdersPerMission_[ &missionType ].insert( pType ) ; 
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::IsMissionAvailable
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
	return availableMissions_.find( &missionType ) !=  availableMissions_.end(); 
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::IsFragOrderAvailableForMission
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const
{
	if( fragOrderType.IsAvailableForAllMissions() )
        return true;

    CIT_FragOrderPerMissionMap it = availableFragOrdersPerMission_.find( &missionType );
    if( it == availableFragOrdersPerMission_.end() )
        return false;

	return it->second.find( &fragOrderType ) != it->second.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetName
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetName() const
{
    return strModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetScriptFile
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetScriptFile() const
{
    return strScript_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetIncludePath
// Created: LDC 2009-05-13
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetIncludePath() const
{
    return strIncludePath_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetDIAType() const
{
    return strDIAType_;
}