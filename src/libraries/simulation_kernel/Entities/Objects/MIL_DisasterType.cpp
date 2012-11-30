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
#include "MIL_Random.h"
#include "Entities/Agents/Units/Humans/PHY_NbcSuit.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

MIL_DisasterType::T_DisasterTypes MIL_DisasterType::disasterTypes_;
unsigned int MIL_DisasterType::thresholdIds_ = 0;

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
    xis >> xml::attribute( "toxicity-exponent", toxicityExponent_ )
        >> xml::start( "protections" )
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
    void ReadWound( xml::xistream& xis, std::map< unsigned int, float >& wounds, const std::string& tag )
    {
        const PHY_HumanWound* wound = PHY_HumanWound::Find( tag );
        if( wound )
            wounds[ wound->GetID() ] = xis.attribute< float >( tag );
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
    attritions_[ thresholdIds_++  ] = boost::tuples::make_tuple( xis.attribute< float >( "value" ), xis.attribute< std::string >( "name", "" ),
                                                                 wounds, xis.attribute< bool >( "contamination" ) );
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

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::GetToxicityExponent
// Created: LGY 2012-11-28
// -----------------------------------------------------------------------------
float MIL_DisasterType::GetToxicityExponent() const
{
    return toxicityExponent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::GetProtectionCoefficient
// Created: LGY 2012-11-28
// -----------------------------------------------------------------------------
float MIL_DisasterType::GetProtectionCoefficient( const PHY_NbcSuit& suit ) const
{
    for( CIT_Protections it = protections_.begin(); it != protections_.end(); ++it )
        if( it->first->GetType() == suit.GetType() )
            return it->second;
    return 1.f;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::GetAttritionThreshold
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
int MIL_DisasterType::GetAttritionThreshold( float dose ) const
{
    int result = -1;
    float current = 0.f;
    for( CIT_Attritions it = attritions_.begin(); it != attritions_.end(); ++it )
    {
        float threshold = it->second.get< 0 >();
        if( dose >= threshold  && threshold >= current )
        {
            result = it->first;
            current = threshold;
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::GetRandomWound
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_DisasterType::GetRandomWound( int threshold ) const
{
    CIT_Attritions it = attritions_.find( threshold );
    if( it != attritions_.end() )
    {
        const T_Wounds& wounds = it->second.get< 2 >();
        const double rRand = MIL_Random::rand_ii( MIL_Random::eWounds );

        double rSumCoefs = 0.;
        const PHY_HumanWound::T_HumanWoundMap& humanWounds = PHY_HumanWound::GetHumanWounds();
        for( PHY_HumanWound::CIT_HumanWoundMap itWound = humanWounds.begin(); itWound != humanWounds.end(); ++itWound )
        {
            const PHY_HumanWound& wound = *itWound->second;
            CIT_Wounds it = wounds.find( wound.GetID() );
            if( it != wounds.end() )
            {
                rSumCoefs += it->second;
                if( rSumCoefs >= rRand )
                    return wound;
            }
        }
    }
    return PHY_HumanWound::notWounded_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DisasterType::IsContaminated
// Created: LGY 2012-11-30
// -----------------------------------------------------------------------------
bool MIL_DisasterType::IsContaminated( float dose ) const
{
    bool result = false;
    for( CIT_Attritions it = attritions_.begin(); it != attritions_.end(); ++it )
        if( dose >= it->second.get< 0 >() )
            result = result || it->second.get< 3 >();
    return result;
}
