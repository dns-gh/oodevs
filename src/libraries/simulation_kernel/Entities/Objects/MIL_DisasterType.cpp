// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DisasterType.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

MIL_DisasterType::T_DisasterTypes MIL_DisasterType::disasterTypes_;

struct MIL_DisasterType::LoadingWrapper
{
    void ReadDisaster( xml::xistream& xis )
    {
        MIL_DisasterType::ReadDisaster( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::Initialize
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void MIL_DisasterType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing disaster types" );
    LoadingWrapper loader;
    xis >> xml::start( "disasters" )
            >> xml::list( "disaster", loader, &LoadingWrapper::ReadDisaster )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::Terminate
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void MIL_DisasterType::Terminate()
{
    disasterTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::ReadDisaster
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void MIL_DisasterType::ReadDisaster( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    MIL_DisasterType* type = new MIL_DisasterType( strName, xis );
    disasterTypes_[ strName ].reset( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType constructor
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
MIL_DisasterType::MIL_DisasterType( const std::string& strName, xml::xistream& /*xis*/ )
    : strName_( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType destructor
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
MIL_DisasterType::~MIL_DisasterType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::GetName
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
const std::string& MIL_DisasterType::GetName() const
{
    return strName_;
}
