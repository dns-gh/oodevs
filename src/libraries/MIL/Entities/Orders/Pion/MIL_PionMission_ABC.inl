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
// Name: MIL_PionMission_ABC::GetPion
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& MIL_PionMission_ABC::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType& MIL_PionMission_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetOrderID
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
uint MIL_PionMission_ABC::GetOrderID() const
{
    return nOrderID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetLima
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
inline
const MIL_Lima* MIL_PionMission_ABC::GetLima( MIL_Lima::E_LimaFunctions nLimaFuncType ) const
{
    for( CIT_LimaFlagedPtrMap itLima = limaMap_.begin(); itLima != limaMap_.end(); ++itLima )
    {
        const MIL_Lima& lima = *itLima->first;
        if( lima.GetLimaFunction() == nLimaFuncType )
            return &lima;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetDirDanger
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PionMission_ABC::GetDirDanger() const
{
    const MT_Vector2D* pTmp = const_cast< MIL_PionMission_ABC& >( *this ).GetVariable( nDIADirectionDangerIdx_ ).ToUserPtr( pTmp );
    if( pTmp )
        return *pTmp;

    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetFuseau
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_PionMission_ABC::GetFuseau() const
{
    return fuseau_;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::SetFuseau
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
inline
void MIL_PionMission_ABC::SetFuseau( const MIL_Fuseau& fuseau )
{
    fuseau_ = fuseau;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetLimas
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const T_LimaFlagedPtrMap& MIL_PionMission_ABC::GetLimas() const
{
    return limaMap_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
inline
bool MIL_PionMission_ABC::GetLimaFlag( const MIL_Lima& lima ) const
{
    CIT_LimaFlagedPtrMap itLima = limaMap_.find( &lima );
    if( itLima == limaMap_.end() )
        return false;
    return itLima->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::SetLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
inline
bool MIL_PionMission_ABC::SetLimaFlag( const MIL_Lima& lima, bool bFlag )
{
    IT_LimaFlagedPtrMap itLima = limaMap_.find( &lima );
    if( itLima == limaMap_.end() )
        return false;

    itLima->second = bFlag;
    return true;
}
