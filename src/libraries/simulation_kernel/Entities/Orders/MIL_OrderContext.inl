//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_PionMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_PionMission_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetDirDanger
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_OrderContext::GetDirDanger() const
{
    return dirDanger_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetFuseau
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_OrderContext::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::AffectFuseau
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
void MIL_OrderContext::AffectFuseau( const MIL_Fuseau& fuseau )
{
    fuseau_ = fuseau;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::AffectDirection
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
inline
void MIL_OrderContext::AffectDirection( const MT_Vector2D& direction )
{
    dirDanger_ = direction;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetLimas
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
const T_LimaVector& MIL_OrderContext::GetLimas() const
{
    return limas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_OrderContext::FindLima( uint nID )
{
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
        if( it->GetID() == nID )
            return &(*it);
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindLima
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_OrderContext::FindLima( const MIL_LimaFunction& func )
{
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
        if( it->HasFunction( func ) )
            return &(*it);
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindNextScheduledLima
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_OrderContext::FindNextScheduledLima()
{
    MIL_LimaOrder* pNextLima = 0;
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
    {
        MIL_LimaOrder& lima = *it;
        if( lima.GetSchedule() == 0 || lima.IsScheduleFlagged() )
            continue;

        if( ( lima.GetSchedule() != 0 ) && ( !pNextLima || lima.GetSchedule() < pNextLima->GetSchedule() ) )
            pNextLima = &lima;
    }
    return pNextLima;
}
