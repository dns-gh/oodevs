// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_InhabitantType.h"
#include "MIL_Inhabitant.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "MT_Tools/MT_Logger.h"
#include "MIL_Schedule_ABC.h"
#include "Urban/PHY_ResourceNetworkType.h"

MIL_InhabitantType::T_InhabitantMap MIL_InhabitantType::inhabitants_;

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Initialize
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing inhabitant types" );
    xis >> xml::start( "populations" )
            >> xml::list( "population", &MIL_InhabitantType::ReadInhabitant )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::ReadInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::ReadInhabitant( xml::xistream& xis )
{
    std::string strName = xis.attribute< std::string >( "name" );
    const MIL_InhabitantType*& pInhabitant = inhabitants_[ strName ];
    if( pInhabitant )
        throw MASA_EXCEPTION( xis.context() + "Inhabitant type already exists" );
    pInhabitant = new MIL_InhabitantType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Terminate
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::Terminate()
{
    for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        delete it->second;
    inhabitants_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_InhabitantType::MIL_InhabitantType( const std::string& strName, xml::xistream& xis )
    : strName_( strName )
{
    nID_ = xis.attribute< unsigned int >( "id" );
    pModel_ = MIL_PopulationType::Find( xis.attribute< std::string >( "associated-crowd" ) );
    if( !pModel_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown crowd type" );
    xis >> xml::optional >> xml::attribute( "angry-crowd-mission", angryCrowdMissionType_ )
        >> xml::start( "safety-level" )
                >> xml::attribute( "gain-per-hour", safetyGainPerHour_ )
                >> xml::attribute( "loss-on-fire", safetyLossOnFire_ )
           >> xml::end
     >> xml::start( "schedule" );
    pXisSchedule_.reset( new xml::xibufferstream( xis ) );
    xis >> xml::end
        >> xml::start( "consumption" )
            >> xml::list( "resource", *this, &MIL_InhabitantType::ReadConsumption )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::ReadConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void MIL_InhabitantType::ReadConsumption( xml::xistream& xis )
{
    const PHY_ResourceNetworkType* resource = PHY_ResourceNetworkType::Find( xis.attribute< std::string >( "type" ) );
    if( !resource )
        throw MASA_EXCEPTION( xis.context() + "Unknown resource network type" );
    consumptions_[ resource ] = xis.attribute< double >( "need" );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_InhabitantType::~MIL_InhabitantType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetAssociatedCrowdType
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const MIL_PopulationType& MIL_InhabitantType::GetAssociatedCrowdType() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetAngryCrowdMissionType
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
const std::string& MIL_InhabitantType::GetAngryCrowdMissionType() const
{
    return angryCrowdMissionType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::InitializeSchedule
// Created: LGY 2011-01-19
// -----------------------------------------------------------------------------
void MIL_InhabitantType::InitializeSchedule( MIL_Schedule_ABC& schedule ) const
{
    schedule.Configure( *pXisSchedule_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetSafetyGainPerHour
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
float MIL_InhabitantType::GetSafetyGainPerHour() const
{
    return safetyGainPerHour_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetSafetyLossOnFire
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
float MIL_InhabitantType::GetSafetyLossOnFire() const
{
    return safetyLossOnFire_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetConsumptions
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
const MIL_InhabitantType::T_ConsumptionsMap& MIL_InhabitantType::GetConsumptions() const
{
    return consumptions_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetName
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const std::string& MIL_InhabitantType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::GetMosID
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
unsigned int MIL_InhabitantType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Find
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const MIL_InhabitantType* MIL_InhabitantType::Find( const std::string& strName )
{
    CIT_InhabitantMap it = inhabitants_.find( strName );
    return it == inhabitants_.end() ? 0 :  it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Find
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const MIL_InhabitantType* MIL_InhabitantType::Find( unsigned int nID )
{
    for( auto it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        if( it->second->nID_ == nID )
            return it->second;
    return 0;
}
