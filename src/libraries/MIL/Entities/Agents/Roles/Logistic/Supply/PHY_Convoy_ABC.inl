// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Supply/PHY_Convoy_ABC.inl $
// $Author: Nld $
// $Modtime: 1/02/05 15:59 $
// $Revision: 1 $
// $Workfile: PHY_Convoy_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetFormingTime
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
inline
uint PHY_Convoy_ABC::GetFormingTime() const
{
    return (uint)( formingTime_( conveyors_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetLoadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_Convoy_ABC::GetLoadingTime() const
{
    return (uint)( loadingTime_( conveyors_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetUnloadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
inline
uint PHY_Convoy_ABC::GetUnloadingTime() const
{
    return (uint)( unloadingTime_( conveyors_.size() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetLoadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_Convoy_ABC::GetLoadingPoint() const
{
    return loadingPoint_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetUnloadingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_Convoy_ABC::GetUnloadingPoint() const
{
    return unloadingPoint_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Convoy_ABC::GetFormingPoint
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PHY_Convoy_ABC::GetFormingPoint() const
{
    return formingPoint_;
}
