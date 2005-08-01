// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationType.inl $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 6 $
// $Workfile: PHY_DotationType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( const std::string& strName ) const
{
    CIT_DotationCategoryMap it = dotationCategories_.find( strName );
    if( it == dotationCategories_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
inline // static
const PHY_DotationType* PHY_DotationType::FindDotationType( const std::string& strName )
{
    CIT_DotationTypeMap itDotationType = dotationTypes_.find( strName );
    if( itDotationType == dotationTypes_.end() )
        return 0;
    return itDotationType->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
inline
const PHY_DotationType* PHY_DotationType::FindDotationType( ASN1T_EnumFamilleDotation nAsnID )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
inline
const PHY_DotationType* PHY_DotationType::FindDotationType( uint nID )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
    {
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
inline // static
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( uint nID )
{
    for( CIT_DotationTypeMap it = dotationTypes_.begin(); it != dotationTypes_.end(); ++it )
    {
        const PHY_DotationCategory* pDotationCategory = it->second->InternalFindDotationCategory( nID );
        if( pDotationCategory ) 
            return pDotationCategory;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetName
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
inline
const std::string& PHY_DotationType::GetName() const
{
    return strName_;
}
          
// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetID
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
inline
uint PHY_DotationType::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetAsnID
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
inline
ASN1T_EnumFamilleDotation PHY_DotationType::GetAsnID() const
{
    return nAsnID_;
}

// =============================================================================
// OPERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::operator==
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
inline
bool PHY_DotationType::operator==( const PHY_DotationType& rhs ) const
{
    return rhs.nType_ == nType_;
}
