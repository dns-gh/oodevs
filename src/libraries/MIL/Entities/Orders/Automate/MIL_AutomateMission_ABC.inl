//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Automate/MIL_AutomateMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_AutomateMission_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetMRT
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
inline
MIL_AutomateMRT& MIL_AutomateMission_ABC::GetMRT()
{
    return mrt_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetAutomate
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_Automate& MIL_AutomateMission_ABC::GetAutomate() const
{
    return automate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetDirDanger
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_AutomateMission_ABC::GetDirDanger() const
{
    const MT_Vector2D* pTmp = const_cast< MIL_AutomateMission_ABC& >( *this ).GetVariable( nDIADirectionDangerIdx_ ).ToUserPtr( pTmp );
    if( pTmp )
        return *pTmp;

    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetFuseau
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_AutomateMission_ABC::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetOrderID

// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
uint MIL_AutomateMission_ABC::GetOrderID() const
{
    return nOrderID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetType
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType& MIL_AutomateMission_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetLimas
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const T_LimaFlagedPtrMap& MIL_AutomateMission_ABC::GetLimas() const
{
    return limaMap_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetLima
// Created: NLD 2004-05-19
// -----------------------------------------------------------------------------
inline
const MIL_Lima* MIL_AutomateMission_ABC::GetLima( MIL_Lima::E_LimaFunctions nLimaFuncType ) const
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
// Name: MIL_AutomateMission_ABC::GetLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateMission_ABC::GetLimaFlag( const MIL_Lima& lima ) const
{
    CIT_LimaFlagedPtrMap itLima = limaMap_.find( &lima );
    if( itLima == limaMap_.end() )
        return false;
    return itLima->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::SetLimaFlag
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateMission_ABC::SetLimaFlag( const MIL_Lima& lima, bool bFlag )
{
    IT_LimaFlagedPtrMap itLima = limaMap_.find( &lima );
    if( itLima == limaMap_.end() )
        return false;

    itLima->second = bFlag;
    return true;
}