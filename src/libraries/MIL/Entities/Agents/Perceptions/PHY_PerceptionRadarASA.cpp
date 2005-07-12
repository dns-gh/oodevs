// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarASA.cpp $
// $Author: Age $
// $Modtime: 4/03/05 14:54 $
// $Revision: 3 $
// $Workfile: PHY_PerceptionRadarASA.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "PHY_PerceptionRadarASA.h"

#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Tools/MIL_Tools.h"

PHY_PerceptionRadarASA::T_RadarTypeVector PHY_PerceptionRadarASA::radarTypes_( eNbrRadarASA, sRadarType() );

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::sRadarType::sRadarType
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
PHY_PerceptionRadarASA::sRadarType::sRadarType()
    : detectableConsumptions_( PHY_ConsumptionType::GetConsumptionTypes().size(), false )
    , rRadius_               ( 0. )
    , rMinHeight_            ( 0. )
    , rMaxHeight_            ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::sRadarType::sRadarType
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
PHY_PerceptionRadarASA::sRadarType::sRadarType( MIL_InputArchive& archive )
    : detectableConsumptions_( PHY_ConsumptionType::GetConsumptionTypes().size(), false )
    , rRadius_               ( 0. )
    , rMinHeight_            ( 0. )
    , rMaxHeight_            ( 0. )    
{
    archive.ReadField( "RayonAction"    , rRadius_   , CheckValueGreaterOrEqual( 0. ) );
    archive.ReadField( "HauteurMinimale", rMinHeight_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadField( "HauteurMaximale", rMaxHeight_, CheckValueGreaterOrEqual( rMinHeight_ ) );

    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

    archive.Section( "ActivitePionsDetectables" );
    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
    for ( PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.begin(); it != consumptionTypes.end(); ++it )
    {
        const PHY_ConsumptionType& conso = *it->second;
        if ( archive.Section( conso.GetName(), MIL_InputArchive::eNothing ) )
        {
            detectableConsumptions_[ conso.GetID() ] = true;
            archive.EndSection();
        }
    }
    archive.EndSection(); // ActivitePionsDetectables
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::sRadarType::Execute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarASA::sRadarType::Execute( PHY_RolePion_Perceiver& perceiver ) const
{
    const MT_Vector2D& vPerceiverPos = perceiver.GetPion().GetRole< PHY_RolePion_Location >().GetPosition();

    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    TER_World::GetWorld().GetListAgentWithinCircle( vPerceiverPos, rRadius_, perceivableAgents );
    for( TER_Agent_ABC::CIT_AgentPtrVector it = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        if( detectableConsumptions_[ target.GetRole< PHY_RoleInterface_Dotations >().GetConsumptionMode().GetID() ] )
        {
            const MT_Float rTargetHeight = target.GetRole< PHY_RoleInterface_Location >().GetHeight();
            if( rTargetHeight <= rMaxHeight_ && rTargetHeight >= rMinHeight_ )
                perceiver.NotifyAgentPerception( target, PHY_PerceptionLevel::recognized_ );
        }
    }
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Initialize
// Created: NLD 2005-02-18
//------------------------------------------------------------------------------
void PHY_PerceptionRadarASA::Initialize( MIL_InputArchive& archive )
{
    archive.Section( "RadarsASA" );

    archive.Section( "RadarHAWK" );
    radarTypes_[ eHawk ] = sRadarType( archive );
    archive.EndSection(); // RadarHAWK

    archive.Section( "RadarMISTRAL" );
    radarTypes_[ eMistral ] = sRadarType( archive );
    archive.EndSection(); // RadarMISTRAL

    archive.Section( "RadarROLAND" );
    radarTypes_[ eRoland ] = sRadarType( archive );
    archive.EndSection(); // RadarROLAND

    archive.EndSection(); // RadarsASA
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Terminate
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarASA::Terminate()
{
    
}

// =============================================================================
// CTOR / DTOR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA constructor
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
PHY_PerceptionRadarASA::PHY_PerceptionRadarASA( PHY_RolePion_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
    , radarsEnabled_    ( eNbrRadarASA, false )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA destructor
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
PHY_PerceptionRadarASA::~PHY_PerceptionRadarASA()
{

}

// =============================================================================
// AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Compute
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarASA::Compute( const MT_Vector2D& /*vPoint*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarASA::Execute( const TER_Agent_ABC::T_AgentPtrVector& /*perceivableAgents*/ )
{   
    for( uint i = 0; i < eNbrRadarASA; ++i )
    {
        if( !radarsEnabled_[ i ] )
            continue;

        assert( i < radarTypes_.size() );
        const sRadarType& radarType = radarTypes_[ i ];

        radarType.Execute( perceiver_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarASA::Compute( const MIL_Agent_ABC& /*agent*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarASA::Compute( const DEC_Knowledge_Agent & /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// =============================================================================
// OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarASA::Compute( const MIL_RealObject_ABC& /*object*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_PerceptionRadarASA::Compute( const DEC_Knowledge_Object& /*knowledge*/ ) const
{
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarASA::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PHY_PerceptionRadarASA::Execute( const TER_DynaObject_ABC::T_DynaObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}