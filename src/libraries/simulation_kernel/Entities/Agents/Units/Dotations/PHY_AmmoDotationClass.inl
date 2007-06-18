// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.inl $
// $Author: Nld $
// $Modtime: 17/03/05 15:08 $
// $Revision: 1 $
// $Workfile: PHY_AmmoDotationClass.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Find
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_AmmoDotationClass* PHY_AmmoDotationClass::Find( int nID )
{
    CIT_TypeMap it = types_.find( nID );
    if( it == types_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Find
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
const PHY_AmmoDotationClass* PHY_AmmoDotationClass::Find( const std::string& strName )
{
    for( CIT_TypeMap it = types_.begin(); it != types_.end(); ++it )
    {
        if( sCaseInsensitiveEqual()( strName, it->second->GetName() ) )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Find
// Created: NLD 2005-07-29
// -----------------------------------------------------------------------------
inline
const PHY_AmmoDotationClass* PHY_AmmoDotationClass::Find( ASN1T_EnumAmmunitionFamily nAsnID )
{
    for( CIT_TypeMap it = types_.begin(); it != types_.end(); ++it )
    {
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::GetID
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
int PHY_AmmoDotationClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::GetAsnID
// Created: NLD 2005-07-29
// -----------------------------------------------------------------------------
inline
ASN1T_EnumAmmunitionFamily PHY_AmmoDotationClass::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::GetName
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
const std::string& PHY_AmmoDotationClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_AmmoDotationClass::operator==( const PHY_AmmoDotationClass& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
inline
bool PHY_AmmoDotationClass::operator!=( const PHY_AmmoDotationClass& rhs ) const
{
    return nType_ != rhs.nType_;
}
