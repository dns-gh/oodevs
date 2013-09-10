// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

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
double MIL_AgentTypePion::GetDistanceAvantPoint( const TerrainData& nType ) const
{
    CIT_DistanceAvantPointMap it = distancesAvantPoints_.find( nType );
    return it == distancesAvantPoints_.end() ? 0. : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetDistanceAvantLima
// Created: JVT 2005-05-10
// -----------------------------------------------------------------------------
inline
double MIL_AgentTypePion::GetDistanceAvantLima() const
{
    return rDistanceAvantLimas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetFeedbackTime
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
inline
double MIL_AgentTypePion::GetFeedbackTime() const
{
    return rFeedbackTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetHumanRepartition
// Created: MMC 2011-10-10
// -----------------------------------------------------------------------------
inline
const MIL_HumanRepartition& MIL_AgentTypePion::GetHumanRepartition() const
{
    return *pHumanRepartition_;
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
    , pModel_( 0 )
    , pUnitType_( new T( xis ) )
    , rDistanceAvantLimas_( 0. )
    , rFeedbackTime_( 0. )
    , pHumanRepartition_( new MIL_HumanRepartition( xis ) )
{
    InitializeRapFor              ( xis );
    InitializeDistancesAvantPoints( xis );
    InitializeModel               ( xis );
}
