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
#include "simulation_kernel/Entities/Populations/MIL_PopulationType.h"
#include "tools/MIL_Tools.h"
#include "MT_Tools/MT_Logger.h"
#include "MIL_AgentServer.h"
#include <xeumeuleu/xml.hpp>

MIL_InhabitantType::T_InhabitantMap MIL_InhabitantType::inhabitants_;

struct MIL_InhabitantType::LoadingWrapper
{
    void ReadInhabitant( xml::xistream& xis )
    {
        MIL_InhabitantType::ReadInhabitant( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Initialize
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing inhabitant types" );
    LoadingWrapper loader;
    xis >> xml::start( "populations" )
            >> xml::list( "population", loader, &LoadingWrapper::ReadInhabitant )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::ReadInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::ReadInhabitant( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

    const MIL_InhabitantType*& pInhabitant = inhabitants_[ strName ];
    if( pInhabitant )
        xis.error( "Inhabitant type already exists" );

    pInhabitant = new MIL_InhabitantType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::Terminate
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_InhabitantType::Terminate()
{
    for( CIT_InhabitantMap it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        delete it->second;
    inhabitants_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_InhabitantType::MIL_InhabitantType( const std::string& strName, xml::xistream& xis )
    : strName_              ( strName )
{
    std::string associatedCrowd; 
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "associated-crowd", associatedCrowd );
    pModel_ = MIL_PopulationType::Find( associatedCrowd );
    if( !pModel_ )
        xis.error( "Unknown crowd type" );
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_InhabitantType::~MIL_InhabitantType()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_InhabitantType::InstanciateInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant& MIL_InhabitantType::InstanciateInhabitant( xml::xistream& xis, MIL_Army_ABC& army ) const
{
    return *new MIL_Inhabitant( xis, *this, army );
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
    for( CIT_InhabitantMap it = inhabitants_.begin(); it != inhabitants_.end(); ++it )
        if( it->second->nID_ == nID )
            return it->second;
    return 0;
}
