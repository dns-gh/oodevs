// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_InfrastructureType.h"
#include "MT_Tools/MT_Logger.h"

PHY_InfrastructureType::T_InfrastructureMap PHY_InfrastructureType::infrastructures_;

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::Initialize
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_InfrastructureType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing infrastructures" );
    xis >> xml::start( "urban" )
            >> xml::optional >> xml::start( "infrastructures" )
                >> xml::list( "infrastructure", &PHY_InfrastructureType::ReadInfrastructure )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::Terminate
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_InfrastructureType::Terminate()
{
    for( CIT_InfrastructureMap itInfrastructure = infrastructures_.begin(); itInfrastructure != infrastructures_.end(); ++itInfrastructure )
        delete itInfrastructure->second;
    infrastructures_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_InfrastructureType* PHY_InfrastructureType::Find( const std::string& strName )
{
    CIT_InfrastructureMap itInfrastructure = infrastructures_.find( strName );
    if( itInfrastructure == infrastructures_.end() )
        return 0;
    return itInfrastructure->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::GetName
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const std::string& PHY_InfrastructureType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_InfrastructureType::PHY_InfrastructureType( const std::string& name, xml::xistream& xis )
    : name_  ( name )
    , symbol_( "infrastructures/" + xis.attribute< std::string >( "symbol" ) )
    , medical_( xis.attribute< bool >( "medical", false ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType destructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_InfrastructureType::~PHY_InfrastructureType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::IsMedical
// Created: ABR 2012-11-22
// -----------------------------------------------------------------------------
bool PHY_InfrastructureType::IsMedical() const
{
    return medical_;
}

// -----------------------------------------------------------------------------
// Name: PHY_InfrastructureType::ReadInfrastructure
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_InfrastructureType::ReadInfrastructure( xml::xistream& xis )
{
    std::string strName = xis.attribute< std::string >( "name" );
    const PHY_InfrastructureType*& pInfrastructure = infrastructures_[ strName ];
    if( pInfrastructure )
        xis.error( "Infrastructure " + strName + " already defined" );
    pInfrastructure = new PHY_InfrastructureType( strName, xis );
}