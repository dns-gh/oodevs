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
#include "Entities/Agents/Units/Humans/PHY_NbcSuit.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
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
MIL_DisasterType::MIL_DisasterType( const std::string& strName, xml::xistream& xis )
    : strName_( strName )
{
    xis >> xml::start( "protections" )
            >> xml::list( "protection", *this, &MIL_DisasterType::ReadProtection )
        >> xml::end
        >> xml::start( "attrition" )
            >> xml::list( "threshold", *this, &MIL_DisasterType::ReadThreshold )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::ReadProtection
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
void MIL_DisasterType::ReadProtection( xml::xistream& xis )
{
    const PHY_NbcSuit* suit = PHY_NbcSuit::Find( xis.attribute< std::string >( "type" ) );
    if( suit )
        protections_[ suit ] = xis.attribute< float >( "value" );
}

namespace
{
    void ReadWound( xml::xistream& xis, std::map< unsigned int, double >& wounds, const std::string& tag )
    {
        const PHY_HumanWound* wound = PHY_HumanWound::Find( tag );
        if( wound )
            wounds[ wound->GetID() ] = xis.attribute< double >( tag );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::ReadThreshold
// Created: LGY 2012-11-28
// -----------------------------------------------------------------------------
void MIL_DisasterType::ReadThreshold( xml::xistream& xis )
{
    T_Wounds wounds;
    ReadWound( xis, wounds, "dead" );
    ReadWound( xis, wounds, "u1" );
    ReadWound( xis, wounds, "u2" );
    ReadWound( xis, wounds, "u3" );
    ReadWound( xis, wounds, "ue" );
    attritions_[ xis.attribute< double >( "value" ) ] = boost::tuples::make_tuple( xis.attribute< std::string >( "name", "" ), wounds,
                                                                                   xis.attribute< bool >( "contamination" ) );
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
