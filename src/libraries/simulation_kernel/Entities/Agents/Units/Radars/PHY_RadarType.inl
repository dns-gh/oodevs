// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarType.inl $
// $Author: Nld $
// $Modtime: 2/05/05 17:35 $
// $Revision: 1 $
// $Workfile: PHY_RadarType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::FindRadarType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const PHY_RadarType* PHY_RadarType::Find( const std::string& strType )
{
    CIT_RadarTypeMap it = radarTypes_.find( strType );
    return it == radarTypes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
const std::string& PHY_RadarType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetClass
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RadarClass& PHY_RadarType::GetClass() const
{
    return class_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetRadius
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RadarType::GetRadius() const
{
    return rRadius_;
}
