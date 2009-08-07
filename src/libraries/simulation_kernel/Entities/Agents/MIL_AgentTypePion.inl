// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentTypePion.inl $
// $Author: Jvt $
// $Modtime: 10/05/05 16:36 $
// $Revision: 8 $
// $Workfile: MIL_AgentTypePion.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Find
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePion::Find( const std::string& strName )
{
    CIT_PionTypeMap it = pionTypes_.find( strName );
    return it == pionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePion::Find( uint nID )
{
    CIT_PionTypeMap it = std::find_if( pionTypes_.begin(), pionTypes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &MIL_AgentTypePion::GetID ), std::select2nd< T_PionTypeMap::value_type >() ) ) );
    return it == pionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetUnitType
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
inline
const PHY_UnitType& MIL_AgentTypePion::GetUnitType() const
{
    assert( pUnitType_ );
    return *pUnitType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetModel
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
const DEC_Model_ABC& MIL_AgentTypePion::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetDistanceAvantPoint
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MT_Float MIL_AgentTypePion::GetDistanceAvantPoint( const TerrainData& nType ) const
{
    CIT_DistanceAvantPointMap it = distancesAvantPoints_.find( nType );
    
    return it == distancesAvantPoints_.end() ? 0. : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetDistanceAvantLima
// Created: JVT 2005-05-10
// -----------------------------------------------------------------------------
inline
MT_Float MIL_AgentTypePion::GetDistanceAvantLima() const
{
    return rDistanceAvantLimas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetRapForIncreasePerTimeStepValue
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
inline
MT_Float MIL_AgentTypePion::GetRapForIncreasePerTimeStepValue() const
{
    return rRapForIncreasePerTimeStepValue_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::operator==
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
inline
bool MIL_AgentTypePion::operator==( const MIL_AgentTypePion& rhs ) const
{
    return this == &rhs; //$$ A changer quand IDs
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
template< typename T >
MIL_AgentTypePion::MIL_AgentTypePion( const std::string& strName, xml::xistream& xis, T* )
    : MIL_AgentType_ABC( strName, xis )
{
    pUnitType_ = new T( xis );

    InitializeRapFor              ( xis );
    InitializeDistancesAvantPoints( xis );
    InitializeModel               ( xis );
    InitializeDiaFunctions        ();
}
