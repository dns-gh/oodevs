// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/orders/pion/MIL_PionMissionType.inl $
// $Author: Nld $
// $Modtime: 4/03/05 17:35 $
// $Revision: 2 $
// $Workfile: MIL_PionMissionType.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: JVT 04-06-18
//-----------------------------------------------------------------------------
template < class Mission >
struct sPionMissionCreator
{
    static MIL_PionMission_ABC& Create( MIL_AgentPion& pion, const MIL_PionMissionType& type )
    {
        return *new Mission( pion, type );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::RegisterMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
template< typename Mission >
void MIL_PionMissionType::RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strBehavior )
{
    MIL_PionMissionType* pNewType = new MIL_PionMissionType( strName, nID, nAsnID, strDIATypeName, strBehavior, &sPionMissionCreator< Mission >::Create, Mission::InitializeDIA );

    if( missionsFromID_.size() < nID + 1 )
        missionsFromID_.resize( nID + 1, 0 );
    const MIL_PionMissionType*& pTypeTmp1 = missionsFromID_[ nID ];
    if( pTypeTmp1 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Pion mission type '%s' is already defined (id=%d)", strName.c_str(), nID ) );
    pTypeTmp1 = pNewType;

    const MIL_PionMissionType*& pTypeTmp2 = missionsFromAsnID_[ nAsnID ];
    if( pTypeTmp2 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Pion mission type '%s' is already defined (asnid=%d)", strName.c_str(), nAsnID ) );
    pTypeTmp2 = pNewType;

    const MIL_PionMissionType*& pTypeTmp3 = missionsFromName_[ strName ];
    if( pTypeTmp3 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Pion mission type '%s' is already defined" ), strName.c_str() );
    pTypeTmp3 = pNewType;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::FindPionMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType* MIL_PionMissionType::FindPionMissionType( const std::string& strName )
{
    CIT_MissionFromNameMap it = missionsFromName_.find( strName );
    if( it == missionsFromName_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::FindPionMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType* MIL_PionMissionType::FindPionMissionType( const ASN1T_Mission_Pion& asnMission )
{
    CIT_MissionFromAsnIDMap it = missionsFromAsnID_.find( asnMission.t );
    if( it == missionsFromAsnID_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::FindPionMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType* MIL_PionMissionType::FindPionMissionType( uint nID )
{
    if( nID >= missionsFromID_.size() )
        return 0;
    return missionsFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::GetID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_PionMissionType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::GetDIAType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PionMissionType::GetDIATypeName() const
{
    return strDIATypeName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::GetBehaviorName
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PionMissionType::GetBehaviorName() const
{
    return strBehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::GetName
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PionMissionType::GetName() const
{
    return strName_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::InstanciateMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_PionMission_ABC& MIL_PionMissionType::InstanciateMission( MIL_AgentPion& pion ) const
{
    return missionAllocator_( pion, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::operator==
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_PionMissionType::operator==( const MIL_PionMissionType& rhs ) const
{
    return nID_ == rhs.nID_;
}


