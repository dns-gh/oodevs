// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_LauncherType.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_LauncherType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::FindLauncherType
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_LauncherType* PHY_LauncherType::FindLauncherType( const std::string& strName )
{
    CIT_LauncherTypeMap it = launcherTypes_.find( strName );
    if( it == launcherTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
const std::string& PHY_LauncherType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::CanDirectFire
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
bool PHY_LauncherType::CanDirectFire() const
{
    return bDirectFire_;
}

// -----------------------------------------------------------------------------
// Name: PHY_LauncherType::CanIndirectFire
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
bool PHY_LauncherType::CanIndirectFire() const
{
    return bIndirectFire_;
}
