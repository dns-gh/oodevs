//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Limit/MIL_LimitManager.inl $
// $Author: Nld $
// $Modtime: 21/10/04 15:54 $
// $Revision: 2 $
// $Workfile: MIL_LimitManager.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::FindLimit
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
const MIL_Limit* MIL_LimitManager::FindLimit( uint nID ) const
{
    CIT_LimitMap itLimit = limits_.find( nID );
    if( itLimit == limits_.end() )
        return 0;
    return itLimit->second;
}


//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::DestroyLimit
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
bool MIL_LimitManager::DestroyLimit( MIL_Limit& limit )
{
    return DestroyLimit( limit.GetID() );
}

//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::CreateLimit
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
inline
MIL_Limit& MIL_LimitManager::CreateLimit( const T_PointVector& points )
{
    MIL_Limit* pLimit = FindLimit( points );
    if( !pLimit )
    {
        pLimit = new MIL_Limit();
        pLimit->Initialize( points );
        limits_.insert( std::make_pair( pLimit->GetID(), pLimit ) );   
    }
    return *pLimit;
}
