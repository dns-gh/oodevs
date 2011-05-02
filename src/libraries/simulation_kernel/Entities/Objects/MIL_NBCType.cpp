//*****************************************************************************
//
// $Created: JVT 03-01-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:31 $
// $Revision: 10 $
// $Workfile: MIL_NbcAgentType.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_NBCType.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

MIL_NBCType::T_NBCTypeMap MIL_NBCType::nbcAgentTypes_;
unsigned int MIL_NBCType::length_;
unsigned int MIL_NBCType::width_;
unsigned int MIL_NBCType::propagationAngle_;
double MIL_NBCType::concentrationIncreaseRate_;
double MIL_NBCType::concentrationDecreaseRate_;
double MIL_NBCType::edgeEffects_;

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Initialize
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_NBCType::Initialize( xml::xistream& xis )
{
    std::set< unsigned int > ids;
    MT_LOG_INFO_MSG( "Initializing nbc agents" );
    xis >> xml::start( "nbc" )
            >> xml::start( "nbc-cell" )
                >> xml::attribute( "length", length_ )
                >> xml::attribute( "width", width_ )
            >> xml::end
            >> xml::start( "propagation" )
                >> xml::attribute( "propagation-angle", propagationAngle_ )
                >> xml::attribute( "concentration-increase-rate" , concentrationIncreaseRate_ )
                >> xml::attribute( "concentration-decrease-rate" , concentrationDecreaseRate_ )
                >> xml::attribute( "edge-effects", edgeEffects_ )
            >> xml::end
            >> xml::start( "agents" )
                >> xml::list( "agent", &ReadAgent )
            >> xml::end
        >> xml::end;
    //We set the propagation angle in radian
    propagationAngle_ *= static_cast< unsigned int >( MT_PI / 180. );
    for( CIT_NBCTypeMap it = nbcAgentTypes_.begin(); it != nbcAgentTypes_.end(); ++it )
        if( ! ids.insert( it->second->GetID() ).second )
            throw std::runtime_error( "NBC agent id of " + it->second->GetName() + " already exists" );
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::ReadClass
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_NBCType::ReadAgent( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    const MIL_NBCType*& pAgent = nbcAgentTypes_[ strName ];
    if( pAgent )
        throw std::runtime_error( "NBC agent " + strName + " already exists" );
    pAgent = new MIL_NBCType( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_NBCType::Terminate()
{
    for( CIT_NBCTypeMap it = nbcAgentTypes_.begin(); it != nbcAgentTypes_.end(); ++it )
        delete it->second;
    nbcAgentTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_NBCType::MIL_NBCType( const std::string& strName, xml::xistream& xis )
    : strName_        ( strName )
    , nID_            ( 0 )
    , bCanContaminate_( false )
    , bCanPoison_     ( false )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::list( "effects", *this, &MIL_NBCType::ReadEffects );
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::ReadExtinguisherAgentEffect
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_NBCType::ReadEffects( xml::xistream& xis )
{
    std::string type;
    xis >> xml::attribute( "type" , type )
        >> xml::attribute( "contaminate", bCanContaminate_ )
        >> xml::attribute( "poison", bCanPoison_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_NBCType::~MIL_NBCType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const MIL_NBCType* MIL_NBCType::Find( const std::string& strName )
{
    CIT_NBCTypeMap it = nbcAgentTypes_.find( strName );
    if( it == nbcAgentTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const MIL_NBCType* MIL_NBCType::Find( unsigned int nID )
{
    for( CIT_NBCTypeMap it = nbcAgentTypes_.begin(); it != nbcAgentTypes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

int MIL_NBCType::ComputeAgentConcentrationEvolution( bool bHasASource, int concentration ) const
{
    if( bHasASource )
        return static_cast< int >( concentration + concentrationIncreaseRate_ );
    return static_cast< int >( concentration - concentrationDecreaseRate_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetName
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const std::string MIL_NBCType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_NBCType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::IsContaminating
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
bool MIL_NBCType::IsContaminating() const
{
    return bCanContaminate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::IsPoisoning
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
bool MIL_NBCType::IsPoisoning() const
{
    return bCanPoison_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetWidth
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_NBCType::GetWidth()
{
    return width_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetLength
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_NBCType::GetLength()
{
    return length_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetConcentrationIncreaseRate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
double MIL_NBCType::GetConcentrationIncreaseRate()
{
    return concentrationIncreaseRate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetConcentrationDecreaseRate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
double MIL_NBCType::GetConcentrationDecreaseRate()
{
    return concentrationDecreaseRate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NBCType::GetPropagationAngle
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_NBCType::GetPropagationAngle()
{
    return propagationAngle_;
}
