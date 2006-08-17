// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_MissionCapacity.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:48 $
// $Revision: 1 $
// $Workfile: PHY_MissionCapacity.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_MissionCapacity* PHY_MissionCapacity::Find( const std::string& strName )
{
    CIT_MissionCapacityMap it = missionCapacity_.find( strName );
    return it == missionCapacity_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_MissionCapacity::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_MissionCapacity::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MissionCapacity::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitCapaciteMission PHY_MissionCapacity::GetAsnID() const
{
    return nAsnID_;
}
