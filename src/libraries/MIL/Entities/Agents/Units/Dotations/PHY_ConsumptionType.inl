// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_ConsumptionType.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 16:21 $
// $Revision: 2 $
// $Workfile: PHY_ConsumptionType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetConsumptionTypes
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType::T_ConsumptionTypeMap& PHY_ConsumptionType::GetConsumptionTypes()
{
    return consumptionTypes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::FindConsumptionType
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType* PHY_ConsumptionType::FindConsumptionType( const std::string& strName )
{
    CIT_ConsumptionTypeMap it = consumptionTypes_.find( strName );
    
    return it == consumptionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::FindConsumptionType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType* PHY_ConsumptionType::FindConsumptionType( uint nID )
{
    // $$$$ JVT : Recherche linéaire, mais n'est utilisé que lors de la reprise de la sim depuis un checkpoint
    CIT_ConsumptionTypeMap it = std::find_if( consumptionTypes_.begin(), consumptionTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_ConsumptionType::GetID ), std::select2nd< T_ConsumptionTypeMap::value_type >() ) ) );

    return it == consumptionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_ConsumptionType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
uint PHY_ConsumptionType::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ConsumptionType::operator <
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
bool PHY_ConsumptionType::operator <( const PHY_ConsumptionType& rhs ) const
{
    return nType_ < rhs.nType_;
}
