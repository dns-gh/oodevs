// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Conduite/MIL_OrderConduiteType.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_OrderConduiteType.inl $
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: sOrderConduiteCreator
// Created: JVT 04-06-18
//-----------------------------------------------------------------------------
template < class OrderConduite >
struct sOrderConduiteCreator
{
    static MIL_OrderConduite_ABC& Create( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    {
        return *new OrderConduite( knowledgeGroup, type );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::RegisterOrderConduiteWithRequest
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
template< typename OrderConduite > static 
void MIL_OrderConduiteType::RegisterOrderConduite( const std::string& strName, uint nConduiteID, uint nAsnConduiteID, const std::string& strConduiteDIATypeName, bool bAvailableForAllMissions, bool bAvailableWithoutMission, uint nAsnRequestID )
{
    MIL_OrderConduiteType* pNewType = new MIL_OrderConduiteType( strName, nConduiteID, nAsnConduiteID, strConduiteDIATypeName, &sOrderConduiteCreator< OrderConduite >::Create, OrderConduite::InitializeDIA, bAvailableForAllMissions, bAvailableWithoutMission, nAsnRequestID );

    if( orderConduitesFromID_.size() < nConduiteID + 1 )
        orderConduitesFromID_.resize( nConduiteID + 1, 0 );
    const MIL_OrderConduiteType*& pTypeTmp1 = orderConduitesFromID_[ nConduiteID ];
    if( pTypeTmp1 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "OrderConduite type '%s' is already defined (id=%d)", strName.c_str(), nConduiteID ) );
    pTypeTmp1 = pNewType;

    const MIL_OrderConduiteType*& pTypeTmp2 = orderConduitesFromAsnID_[ nAsnConduiteID ];
    if( pTypeTmp2 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "OrderConduite type '%s' is already defined (asnid=%d)", strName.c_str(), nAsnConduiteID ) );
    pTypeTmp2 = pNewType;

    const MIL_OrderConduiteType*& pTypeTmp3 = orderConduitesFromName_[ strName ];
    if( pTypeTmp3 ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "OrderConduite type '%s' is already defined" ), strName.c_str() );
    pTypeTmp3 = pNewType;
}


// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::FindOrderConduiteType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_OrderConduiteType* MIL_OrderConduiteType::FindOrderConduiteType( const std::string& strName )
{
    CIT_OrderConduiteFromNameMap it = orderConduitesFromName_.find( strName );
    if( it == orderConduitesFromName_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::FindOrderConduiteType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_OrderConduiteType* MIL_OrderConduiteType::FindOrderConduiteType( const ASN1T_MsgOrderConduite_order_conduite& asn )
{
    CIT_OrderConduiteFromAsnIDMap it = orderConduitesFromAsnID_.find( asn.t );
    if( it == orderConduitesFromAsnID_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::FindOrderConduiteType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_OrderConduiteType* MIL_OrderConduiteType::FindOrderConduiteType( uint nID )
{
    if( nID >= orderConduitesFromID_.size() )
        return 0;
    return orderConduitesFromID_[ nID ];
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::IsAvailableForAllMissions
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_OrderConduiteType::IsAvailableForAllMissions() const
{
    return bAvailableForAllMissions_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::IsAvailableWithoutMission
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_OrderConduiteType::IsAvailableWithoutMission() const
{
    return bAvailableWithoutMission_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::InstanciateOrderConduite
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_OrderConduite_ABC& MIL_OrderConduiteType::InstanciateOrderConduite( const MIL_KnowledgeGroup& knowledgeGroup ) const
{
    return orderConduiteAllocator_( knowledgeGroup, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::CanBeRequested
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
bool MIL_OrderConduiteType::CanBeRequested() const
{
    return nAsnRequestID_ != (uint)-1;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::operator==
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool MIL_OrderConduiteType::operator==( const MIL_OrderConduiteType& rhs ) const
{
    return rhs.nConduiteID_ == nConduiteID_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::GetConduiteID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_OrderConduiteType::GetConduiteID() const
{
    return nConduiteID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::GetConduiteDIATypeName
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const std::string& MIL_OrderConduiteType::GetConduiteDIATypeName() const
{
    return strConduiteDIATypeName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteType::GetAsnRequestID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_OrderConduiteType::GetAsnRequestID() const
{
    return nAsnRequestID_;
}
