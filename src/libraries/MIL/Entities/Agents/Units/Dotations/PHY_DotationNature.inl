// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationNature.inl $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 6 $
// $Workfile: PHY_DotationNature.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::GetName
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
const std::string& PHY_DotationNature::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::Find
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
const PHY_DotationNature* PHY_DotationNature::Find( const std::string& strName )
{
    CIT_DotationNatureMap it = natures_.find( strName );
    if( it == natures_.end() )
        return 0;
    return it->second;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::operator==
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
bool PHY_DotationNature::operator==( const PHY_DotationNature& rhs ) const
{
    return nID_ == rhs.nID_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_DotationNature::operator!=
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
bool PHY_DotationNature::operator!=( const PHY_DotationNature& rhs ) const
{
    return nID_ != rhs.nID_;
}
