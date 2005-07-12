// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarClass.inl $
// $Author: Nld $
// $Modtime: 2/05/05 18:20 $
// $Revision: 1 $
// $Workfile: PHY_RadarClass.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::FindRadarClass
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const PHY_RadarClass* PHY_RadarClass::Find( const std::string& strClass )
{
    CIT_RadarClassMap it = radarClasses_.find( strClass );
    return it == radarClasses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::Find
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RadarClass* PHY_RadarClass::Find( uint nID )
{
    for( CIT_RadarClassMap it = radarClasses_.begin(); it != radarClasses_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetID
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
uint PHY_RadarClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::IsActive
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
bool PHY_RadarClass::IsActive() const
{
    return bIsActive_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetName
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const std::string& PHY_RadarClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetNbrClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
uint PHY_RadarClass::GetNbrClasses()
{
    return radarClasses_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::GetRadarClasses
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RadarClass::T_RadarClassMap& PHY_RadarClass::GetRadarClasses()
{
    return radarClasses_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::operator==
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
bool PHY_RadarClass::operator==( const PHY_RadarClass& rhs ) const
{
    return rhs.nType_ == nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarClass::operator!=
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
bool PHY_RadarClass::operator!=( const PHY_RadarClass& rhs ) const
{
    return rhs.nType_ != nType_;
}
