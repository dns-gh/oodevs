// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateMissionType.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:10 $
// $Revision: 3 $
// $Workfile: MIL_AutomateMissionType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetMoveToRefugeeCampMissionType
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType& MIL_AutomateMissionType::GetMoveToRefugeeCampMissionType()
{
    assert( pMoveToRefugeeCampMissionType_ );
    return *pMoveToRefugeeCampMissionType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetSurrenderingMissionType
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType& MIL_AutomateMissionType::GetSurrenderingMissionType()
{
    assert( pSurrenderingMissionType_ );
    return *pSurrenderingMissionType_;
}

//-----------------------------------------------------------------------------
// Name: sMissionCreator
// Created: JVT 04-06-18
//-----------------------------------------------------------------------------
template < class Mission >
struct sAutomateMissionCreator
{
    static MIL_AutomateMission_ABC& Create( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    {
        return *new Mission( automate, type );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::RegisterMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
template< typename Mission >
void MIL_AutomateMissionType::RegisterMission( const std::string& strName, uint nID, uint nAsnID, const std::string& strDIATypeName, const std::string& strMrtBehavior, const std::string& strConduiteBehavior )
{
    MIL_AutomateMissionType* pNewType = new MIL_AutomateMissionType( strName, nID, nAsnID, strDIATypeName, strMrtBehavior, strConduiteBehavior, &sAutomateMissionCreator< Mission >::Create, Mission::InitializeDIA );

    if( missionsFromID_.size() < nID + 1 )
        missionsFromID_.resize( nID + 1, 0 );
    const MIL_AutomateMissionType*& pTypeTmp1 = missionsFromID_[ nID ];
    if( pTypeTmp1 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined (id=%d)", strName.c_str(), nID ) );
    pTypeTmp1 = pNewType;

    const MIL_AutomateMissionType*& pTypeTmp2 = missionsFromAsnID_[ nAsnID ];
    if( pTypeTmp2 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined (asnid=%d)", strName.c_str(), nAsnID ) );
    pTypeTmp2 = pNewType;

    const MIL_AutomateMissionType*& pTypeTmp3 = missionsFromName_[ strName ];
    if( pTypeTmp3 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Automata mission type '%s' is already defined" ), strName.c_str() );
    pTypeTmp3 = pNewType;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::FindAutomateMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType* MIL_AutomateMissionType::FindAutomateMissionType( const std::string& strName )
{
    CIT_MissionFromNameMap it = missionsFromName_.find( strName );
    if( it == missionsFromName_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::FindAutomateMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType* MIL_AutomateMissionType::FindAutomateMissionType( const ASN1T_Mission_Automate& asnMission )
{
    CIT_MissionFromAsnIDMap it = missionsFromAsnID_.find( asnMission.t );
    if( it == missionsFromAsnID_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::FindAutomateMissionType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType* MIL_AutomateMissionType::FindAutomateMissionType( uint nID )
{
    if( nID >= missionsFromID_.size() )
        return 0;
    return missionsFromID_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_AutomateMissionType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetDIAType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetDIATypeName() const
{
    return strDIATypeName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetMrtBehaviorName
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetMrtBehaviorName() const
{
    return strMrtBehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetConduiteBehaviorName
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetConduiteBehaviorName() const
{
    return strConduiteBehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetName
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetName() const
{
    return strName_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::InstanciateMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_AutomateMission_ABC& MIL_AutomateMissionType::InstanciateMission( MIL_Automate& automate ) const
{
    return missionAllocator_( automate, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::operator==
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateMissionType::operator==( const MIL_AutomateMissionType& rhs ) const
{
    return nID_ == rhs.nID_;
}


