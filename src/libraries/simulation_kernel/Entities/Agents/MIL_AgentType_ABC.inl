// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentType_ABC.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:49 $
// $Revision: 7 $
// $Workfile: MIL_AgentType_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetID
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
inline
uint MIL_AgentType_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetNatureLevel
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureLevel& MIL_AgentType_ABC::GetNatureLevel() const
{
    assert( pNatureLevel_ );
    return *pNatureLevel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetNatureAtlas
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureAtlas& MIL_AgentType_ABC::GetNatureAtlas() const
{
    assert( pNatureAtlas_ );
    return *pNatureAtlas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AgentType_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsMilitia
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AgentType_ABC::IsMilitia() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsTerrorist
// Created: NLD 2006-02-23
// -----------------------------------------------------------------------------
inline
bool MIL_AgentType_ABC::IsTerrorist() const
{
    return false;
}
          
// -----------------------------------------------------------------------------
// Name: MIL_AgentType_ABC::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AgentType_ABC::IsRefugee() const
{
    return false;
}
