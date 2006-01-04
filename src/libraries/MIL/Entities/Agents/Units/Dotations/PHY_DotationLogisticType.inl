// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.inl $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 6 $
// $Workfile: PHY_DotationLogisticType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( const std::string& strName )
{
    CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.find( strName );
    if( it == dotationLogisticTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( uint nID )
{
    for( CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.begin(); it != dotationLogisticTypes_.end(); ++it )
    {
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetDotationLogisticTypes
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
const PHY_DotationLogisticType::T_DotationLogisticTypeMap& PHY_DotationLogisticType::GetDotationLogisticTypes()
{
    return dotationLogisticTypes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetID
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
uint PHY_DotationLogisticType::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetName
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
inline
const std::string& PHY_DotationLogisticType::GetName() const
{
    return strName_;
}
