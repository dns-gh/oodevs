// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Created: RFT 24/04/2008
// Name: MIL_FireClass.cpp
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------


#include "simulation_kernel_pch.h"
#include "MIL_FireClass.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"

#include <xeumeuleu/xml.h>

MIL_FireClass::T_FireClassMap MIL_FireClass::classes_;

// =============================================================================
// FACTORY
// Created: RFT 19/05/2008
// Modified: none
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadClass
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadClass( xml::xistream& xis )
{    
    std::string strName;

    xis >> xml::attribute( "name", strName );

    const MIL_FireClass*& pClass = classes_[ strName ];
    if( pClass )
        throw std::runtime_error( "Fire Class " + strName + " already exists" );
    pClass = new MIL_FireClass( strName, xis );        
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Initialize
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::Initialize( xml::xistream& xis )
{
    std::set< uint > ids;
    MT_LOG_INFO_MSG( "Initializing fire classes" );
    
    xis >> xml::start( "fire-classes" )
			>> xml::list( "class", &ReadClass )
        >> xml::end();

    for( CIT_FireClassMap it = classes_.begin(); it != classes_.end(); ++it )
        if( ! ids.insert( it->second->GetID() ).second )
            throw std::runtime_error( "Fire Class id of " + it->second->GetName() + " already exists" );
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicFireClass constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireClass::MIL_FireClass( const std::string& strName, xml::xistream& xis )
	: strName_              ( strName )
	, nID_                  ( 0 )
	, tempThreshold_        ( 0 )
	, defaultHeat_          ( 0 )
	, heatMax_              ( 0 )
	, increaseRate_         ( 0 )
	, decreaseRate_         ( 0 )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "defaultheat", defaultHeat_ )
        >> xml::attribute( "heatmax", heatMax_ )
        >> xml::attribute( "increaserate", increaseRate_ )
        >> xml::attribute( "decreaserate", decreaseRate_ )
        >> xml::attribute( "tempthreshold", tempThreshold_ )
        >> xml::start( "extinguisher-agents" )
            >> xml::list( "agent", *this, &MIL_FireClass::ReadExtinguisherAgentEffect )
        >> xml::end();    
}

namespace 
{
    MIL_FireClass::E_FireExtinguisherAgent StringToE_FireExtinguisherAgent( const std::string& nature )
    {
        MIL_FireClass::E_FireExtinguisherAgent extinguisherAgent = MIL_FireClass::eEauEnMasse;

        if( nature == "EauEnMasse" )
            extinguisherAgent = MIL_FireClass::eEauEnMasse;
        else if( nature == "EauEnPulverisation" )
            extinguisherAgent = MIL_FireClass::eEauEnPulverisation;
        else if( nature == "Mousses" )
            extinguisherAgent = MIL_FireClass::eMousses;
        else if( nature == "Poudres" )
            extinguisherAgent = MIL_FireClass::ePoudres;
        else if( nature == "CO2" )
            extinguisherAgent = MIL_FireClass::eCO2;
        else if( nature == "Solide" )
            extinguisherAgent = MIL_FireClass::eSolide;
        else if( nature == "HydrocarburesHalogenes" )
            extinguisherAgent = MIL_FireClass::eHydrocarburesHalogenes;
       return extinguisherAgent;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::ReadExtinguisherAgentEffect
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::ReadExtinguisherAgentEffect( xml::xistream& xis )
{
    unsigned int effect;
    std::string nature;
    E_FireExtinguisherAgent extinguisherAgent; 
    xis >> xml::attribute( "effect", effect )
        >> xml::attribute( "nature", nature );

    extinguisherAgent = StringToE_FireExtinguisherAgent( nature );
    extinguisherAgentEffect_.insert( std::make_pair( extinguisherAgent, effect ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_FireClass::Terminate()
{
    for( CIT_FireClassMap it = classes_.begin(); it != classes_.end(); ++it )
        delete it->second;
    classes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------

const MIL_FireClass* MIL_FireClass::Find( const std::string& strName )
{
    CIT_FireClassMap it = classes_.find( strName );
    if( it == classes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const MIL_FireClass* MIL_FireClass::Find( uint nID )
{
    for( CIT_FireClassMap it = classes_.begin(); it != classes_.end(); ++it )
    {        
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicFireClassA destructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_FireClass::~MIL_FireClass()
{
	//Nothing
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
uint MIL_FireClass::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FireClass::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const std::string& MIL_FireClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: GetDefaultHeat()
// Created: RFT 22/05/2008
// Modified: none
// -----------------------------------------------------------------------------
int MIL_FireClass::GetDefaultHeat() const
{
    return defaultHeat_;
}


// =============================================================================
// HEAT EVOLUTION
// Created: RFT 19/05/2008
// Modified: none
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ComputeHeatEvolution
// Created: RFT 28/04/2008
// Modified: none
// -----------------------------------------------------------------------------
int	MIL_FireClass::ComputeHeatEvolution( int heat, unsigned int timeOfCreation, unsigned int timeOflastUpdate ) const
{
    //Temporal informations acquisition
    unsigned int time = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    unsigned int timeSinceCreation = time - timeOfCreation;

    //New heat computation depending on fire phasis
    if( timeSinceCreation < tempThreshold_ && heat < heatMax_ )
        heat =  heat + ( int )( ( time - timeOflastUpdate )*increaseRate_ );
    else if( timeSinceCreation > tempThreshold_ )
        heat = heat - ( int )( ( time - timeOflastUpdate )*decreaseRate_ );
       
    return heat;
}

// -----------------------------------------------------------------------------
// Name: ComputeHeatEvolutionWhenTryingToExtinguish
// Created: RFT 28/04/2008
// Modified: none
// -----------------------------------------------------------------------------
int MIL_FireClass::ComputeHeatWhenExtinguishing( int heat, E_FireExtinguisherAgent extinguisherAgent, int numberOfFireHoses ) const
{
    CIT_ExtinguisherAgentEffectMap iter = extinguisherAgentEffect_.find( extinguisherAgent );
    if( iter != extinguisherAgentEffect_.end() )
        heat -= numberOfFireHoses * iter->second;
    return heat;
}


MIL_FireClass::T_EvaluationResult MIL_FireClass::Evaluate( const PHY_Weapon& weapon ) const
{
    //If the extinguisher agent isn't found return extinguisher agent effect = -1
    //If the extinguisher agent isn't found return fire hose range = -1

    MIL_FireClass::T_EvaluationResult result; 
    result.agent_ = StringToE_FireExtinguisherAgent( weapon.GetDotationCategory().GetNature().GetName() );
    CIT_ExtinguisherAgentEffectMap iter = extinguisherAgentEffect_.find( result.agent_ );
    if( iter != extinguisherAgentEffect_.end() )
    {
        result.score_ = iter->second;
        result.range_ = weapon.GetMaxRangeToIndirectFire();
    }
    return result;
}

// =============================================================================
// WOUND
// Created: RFT 19/05/2008
// Modified: none
// =============================================================================

// -----------------------------------------------------------------------------
// Name: GetRandomWound() const
// Created: RFT 28/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
const PHY_HumanWound& MIL_FireClass::GetRandomWound() const
{
    return PHY_HumanWound::notWounded_;
}
