// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObjectType.inl $
// $Author: Nld $
// $Modtime: 5/04/05 11:37 $
// $Revision: 4 $
// $Workfile: MIL_RealObjectType.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetObjectTypesToAvoid
// Created: NLD 2005-07-01
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectTypeFilter& MIL_RealObjectType::GetDangerousObjectTypes()
{
    assert( pDangerousObjectTypes_ );
    return *pDangerousObjectTypes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetObjectTypes
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectType::T_ObjectTypeMap& MIL_RealObjectType::GetObjectTypes()
{
    return objectTypes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Find
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectType* MIL_RealObjectType::Find( const std::string& strName )
{
    for( CIT_ObjectTypeMap it = objectTypes_.begin(); it != objectTypes_.end(); ++it )
        if( sCaseInsensitiveEqual()( it->second->GetName(), strName ) )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Find
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectType* MIL_RealObjectType::Find( ASN1T_EnumObjectType nAsnID )
{
    for( CIT_ObjectTypeMap it = objectTypes_.begin(); it != objectTypes_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Find
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectType* MIL_RealObjectType::Find( uint nID )
{
    CIT_ObjectTypeMap it = objectTypes_.find( nID );
    if( it == objectTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetExitingPopulationDensity
// Created: NLD 2006-02-23
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetExitingPopulationDensity() const
{
    return rExitingPopulationDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetID
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
uint MIL_RealObjectType::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetNbrMaxAnimators
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
inline
uint MIL_RealObjectType::GetNbrMaxAnimators() const
{
    return nNbrMaxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetAsnID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType MIL_RealObjectType::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetName
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const std::string& MIL_RealObjectType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::IsInitialized
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::IsInitialized() const
{
    return bInitialized_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetIDManager
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
MIL_MOSIDManager& MIL_RealObjectType::GetIDManager() const
{
    assert( pIDManager_ );
    return *pIDManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetAvoidanceDistance
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetAvoidanceDistance() const
{
    return rAvoidanceDistance_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::CanBeMined
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::CanBeMined() const
{
    return bCanBeMined_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::CanBeReservedObstacle
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::CanBeReservedObstacle() const
{
    return bCanBeReservedObstacle_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::CanBeBypassed
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::CanBeBypassed() const
{
    return bCanBeBypassed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetMaxInteractionHeight
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetMaxInteractionHeight() const
{
    return rMaxInteractionHeight_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetDotationCategoryForConstruction
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory* MIL_RealObjectType::GetDotationCategoryForConstruction() const
{
    return pDotationCategoryForConstruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetDotationCategoryForMining
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
const PHY_DotationCategory* MIL_RealObjectType::GetDotationCategoryForMining() const
{
    return pDotationCategoryForMining_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetNbrDotationForConstruction
// Created: NLD 2006-09-15
// -----------------------------------------------------------------------------
inline
uint MIL_RealObjectType::GetNbrDotationForConstruction() const
{
    return nNbrDotationForConstruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetNbrDotationForMining
// Created: NLD 2006-09-15
// -----------------------------------------------------------------------------
inline
uint MIL_RealObjectType::GetNbrDotationForMining() const
{
    return nNbrDotationForMining_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InstanciateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_RealObjectType::InstanciateObject( uint nID, MIL_Army& army ) const
{
    return objectInstanciator_( *this, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetDefaultSpeedWhenBypassed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetDefaultSpeedWhenBypassed() const
{
    return rDefaultSpeedWhenBypassed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetDefaultSpeedWhenNotBypassed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetDefaultSpeedWhenNotBypassed() const
{
    return rDefaultSpeedWhenNotBypassed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetDefaultConsumptionMode
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
inline
const PHY_ConsumptionType& MIL_RealObjectType::GetDefaultConsumptionMode() const
{
    assert( pDefaultConsumptionMode_ );
    return *pDefaultConsumptionMode_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetPopulationAttritionPH
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetPopulationAttritionPH() const
{
    return rPopulationAttritionPH_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetPopulationAttritionSurface
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObjectType::GetPopulationAttritionSurface() const
{
    return rPopulationAttritionSurface_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::operator==
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::operator==( const MIL_RealObjectType& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::operator!=
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
inline
bool MIL_RealObjectType::operator!=( const MIL_RealObjectType& rhs ) const
{
    return nType_ != rhs.nType_;
}
