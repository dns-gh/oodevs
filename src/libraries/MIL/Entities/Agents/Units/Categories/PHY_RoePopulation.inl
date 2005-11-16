// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_RoePopulation.inl $
// $Author: Jvt $
// $Modtime: 9/12/04 16:49 $
// $Revision: 2 $
// $Workfile: PHY_RoePopulation.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetRoePopulations
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
inline
const PHY_RoePopulation::T_RoePopulationMap& PHY_RoePopulation::GetRoePopulations()
{
    return roePopulations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_RoePopulation* PHY_RoePopulation::Find( const std::string& strName )
{
    CIT_RoePopulationMap it = roePopulations_.find( strName );
    return it == roePopulations_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Find
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
inline
const PHY_RoePopulation* PHY_RoePopulation::Find( uint nID )
{
    for( CIT_RoePopulationMap it = roePopulations_.begin(); it != roePopulations_.end(); ++it )
    {
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& PHY_RoePopulation::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint PHY_RoePopulation::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetAsnID
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
inline
ASN1T_EnumRoePopulation PHY_RoePopulation::GetAsnID() const
{
    return nAsnID_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::operator==
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
inline
bool PHY_RoePopulation::operator==( const PHY_RoePopulation& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::operator!=
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
inline
bool PHY_RoePopulation::operator!=( const PHY_RoePopulation& rhs ) const
{
    return nType_ != rhs.nType_;
}
