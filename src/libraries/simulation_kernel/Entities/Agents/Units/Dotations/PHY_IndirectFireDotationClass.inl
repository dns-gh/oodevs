// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.inl $
// $Author: Nld $
// $Modtime: 17/03/05 14:58 $
// $Revision: 1 $
// $Workfile: PHY_IndirectFireDotationClass.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Find
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_IndirectFireDotationClass* PHY_IndirectFireDotationClass::Find( int nID )
{
    CIT_TypeMap it = types_.find( nID );
    if( it == types_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Find
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
const PHY_IndirectFireDotationClass* PHY_IndirectFireDotationClass::Find( const std::string& strName )
{
    for( CIT_TypeMap it = types_.begin(); it != types_.end(); ++it )
    {
        if( sCaseInsensitiveEqual()( strName, it->second->GetName() ) )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::InstanciateWeaponDataType
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
PHY_DotationCategory_IndirectFire_ABC& PHY_IndirectFireDotationClass::InstanciateDotationCategory( const PHY_DotationCategory& dotationCategory, xml::xistream& xis ) const
{
    return typeInstancier_( *this, dotationCategory, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::GetID
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
int PHY_IndirectFireDotationClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::GetName
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
const std::string& PHY_IndirectFireDotationClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireDotationClass::operator==( const PHY_IndirectFireDotationClass& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireDotationClass::operator!=( const PHY_IndirectFireDotationClass& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::NeedPH
// Created: NLD 2005-08-03
// -----------------------------------------------------------------------------
inline
bool PHY_IndirectFireDotationClass::NeedPH() const
{
    return bNeedPH_;
}
