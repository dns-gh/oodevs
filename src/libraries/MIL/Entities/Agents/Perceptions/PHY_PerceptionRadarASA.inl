// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionRadarASA.inl $
// $Author: Nld $
// $Modtime: 23/02/05 14:41 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionRadarASA.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::EnableRadar
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionRadarASA::EnableRadar( uint nType )
{
    if( nType < radarsEnabled_.size() )
        radarsEnabled_[ nType ] = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::DisableRadar
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionRadarASA::DisableRadar( uint nType )
{
    if( nType < radarsEnabled_.size() )
        radarsEnabled_[ nType ] = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::HasRadarToHandle
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRadarASA::HasRadarToHandle() const
{
    return std::find( radarsEnabled_.begin(), radarsEnabled_.end(), true ) != radarsEnabled_.end();
}
