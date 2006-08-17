// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_PopulationMissionType.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:10 $
// $Revision: 3 $
// $Workfile: MIL_PopulationMissionType.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: JVT 04-06-18
//-----------------------------------------------------------------------------
template < class Mission >
struct sPopulationMissionCreator
{
    static MIL_PopulationMission_ABC& Create( MIL_Population& population, const MIL_PopulationMissionType& type )
    {
        return *new Mission( population, type );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::RegisterMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
template< typename Mission >
void MIL_PopulationMissionType::RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior )
{
    MIL_PopulationMissionType* pNewType = new MIL_PopulationMissionType( strName, nID, nAsnID, strDIATypeName, strBehavior, &sPopulationMissionCreator< Mission >::Create, Mission::InitializeDIA );

    if( missionsFromID_.size() < nID + 1 )
        missionsFromID_.resize( nID + 1, 0 );
    const MIL_PopulationMissionType*& pTypeTmp1 = missionsFromID_[ nID ];
    if( pTypeTmp1 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined (id=%d)", strName.c_str(), nID ) );
    pTypeTmp1 = pNewType;

    const MIL_PopulationMissionType*& pTypeTmp2 = missionsFromAsnID_[ nAsnID ];
    if( pTypeTmp2 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined (asnid=%d)", strName.c_str(), nAsnID ) );
    pTypeTmp2 = pNewType;

    const MIL_PopulationMissionType*& pTypeTmp3 = missionsFromName_[ strName ];
    if( pTypeTmp3 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined" ), strName.c_str() );
    pTypeTmp3 = pNewType;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::FindPopulationMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PopulationMissionType* MIL_PopulationMissionType::FindPopulationMissionType( const std::string& strName )
{
    CIT_MissionFromNameMap it = missionsFromName_.find( strName );
    if( it == missionsFromName_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::FindPopulationMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PopulationMissionType* MIL_PopulationMissionType::FindPopulationMissionType( const ASN1T_Mission_Population& asnMission )
{
    CIT_MissionFromAsnIDMap it = missionsFromAsnID_.find( asnMission.t );
    if( it == missionsFromAsnID_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::FindPopulationMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PopulationMissionType* MIL_PopulationMissionType::FindPopulationMissionType( uint nID )
{
    if( nID >= missionsFromID_.size() )
        return 0;
    return missionsFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::GetID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationMissionType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::GetDIAType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PopulationMissionType::GetDIATypeName() const
{
    return strDIATypeName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::GetBehaviorName
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PopulationMissionType::GetBehaviorName() const
{
    return strBehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::GetName
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PopulationMissionType::GetName() const
{
    return strName_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::InstanciateMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_PopulationMission_ABC& MIL_PopulationMissionType::InstanciateMission( MIL_Population& population ) const
{
    return missionAllocator_( population, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::operator==
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationMissionType::operator==( const MIL_PopulationMissionType& rhs ) const
{
    return nID_ == rhs.nID_;
}


