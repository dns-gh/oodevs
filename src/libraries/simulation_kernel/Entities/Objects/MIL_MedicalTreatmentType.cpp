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
// Name: MIL_MedicalTreatmentType.cpp
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------


#include "simulation_kernel_pch.h"
#include "MIL_MedicalTreatmentType.h"
#include <xeumeuleu/xml.h>
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"

MIL_MedicalTreatmentType::T_MedicalTreatmentTypeMap MIL_MedicalTreatmentType::types_;

// =============================================================================
// FACTORY
// Created: RFT 19/05/2008
// Modified: none
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::ReadClass
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::ReadMedicalTreatment( xml::xistream& xis )
{    
    std::string strName;

    xis >> xml::attribute( "name", strName );

    const MIL_MedicalTreatmentType*& pType = types_[ strName ];
    if( pType )
        throw std::runtime_error( "Medical Treatment of Type " + strName + " already exists" );
    pType = new MIL_MedicalTreatmentType( strName, xis );        
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Initialize
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::Initialize( xml::xistream& xis )
{
    std::set< uint > ids;
    MT_LOG_INFO_MSG( "Initializing Medical Treatment Types" );
    
    xis >> xml::start( "medical-treatments" )
			>> xml::list( "medical-treatment", &ReadMedicalTreatment )
        >> xml::end();

    for( CIT_MedicalTreatmentTypeMap it = types_.begin(); it != types_.end(); ++it )
        if( ! ids.insert( it->second->GetID() ).second )
            throw std::runtime_error( "Medical treatment type id of " + it->second->GetName() + " already exists" );
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicMedicalTreatmentType constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::MIL_MedicalTreatmentType( const std::string& strName, xml::xistream& xis )
	: strName_              ( strName )
	, nID_                  ( 0 )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::start( "injuries" )
            >> xml::list( "injury", *this, &MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect )
        >> xml::end();    
}

namespace 
{
    MIL_MedicalTreatmentType::E_Injuries StringToE_Injuries( const std::string& category )
    {
        MIL_MedicalTreatmentType::E_Injuries injury = MIL_MedicalTreatmentType::eUA;

        if( category == "UA" )
            injury = MIL_MedicalTreatmentType::eUA;
        else if( category == "UR" )
            injury = MIL_MedicalTreatmentType::eUR;
        return injury;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect( xml::xistream& xis )
{
    std::string category;
    T_HealingTimes times;
    E_Injuries injury;

    xis >> xml::attribute( "category" , category );
    injury = StringToE_Injuries( category );

    tools::ReadTimeAttribute( xis , "treatment-time" , times.first );
    if( times.first <= 0 )
        xis.error( "treatment-time <= 0" );
    times.first = ( int ) MIL_Tools::ConvertSecondsToSim( times.first );

    tools::ReadTimeAttribute( xis , "hospitalisation-time" , times.second );
    if( times.second <= 0 )
        xis.error( "hospitalisation-time <= 0" );
    times.second = ( int ) MIL_Tools::ConvertSecondsToSim( times.second );
   
    medicalTreatmentEffect_.insert( std::make_pair( injury, times ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::Terminate()
{
    for( CIT_MedicalTreatmentTypeMap it = types_.begin(); it != types_.end(); ++it )
        delete it->second;
    types_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------

const MIL_MedicalTreatmentType* MIL_MedicalTreatmentType::Find( const std::string& strName )
{
    CIT_MedicalTreatmentTypeMap it = types_.find( strName );
    if( it == types_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Find
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const MIL_MedicalTreatmentType* MIL_MedicalTreatmentType::Find( uint nID )
{
    for( CIT_MedicalTreatmentTypeMap it = types_.begin(); it != types_.end(); ++it )
    {        
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicMedicalTreatmentTypeA destructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::~MIL_MedicalTreatmentType()
{
	//Nothing
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
uint MIL_MedicalTreatmentType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
const std::string& MIL_MedicalTreatmentType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetTreatmentTime
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
int MIL_MedicalTreatmentType::GetTreatmentTime( int injury ) const
{
    CIT_MedicalTreatmentEffectMap iter = medicalTreatmentEffect_.find( ( E_Injuries )injury );
    if( iter != medicalTreatmentEffect_.end() )
        return iter->second.first;
    else
        return -1;//IF THERE IS AN ERROR
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetHospitalisationTime
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
int MIL_MedicalTreatmentType::GetHospitalisationTime( int injury ) const
{
    CIT_MedicalTreatmentEffectMap iter = medicalTreatmentEffect_.find( ( E_Injuries )injury );
    if( iter != medicalTreatmentEffect_.end() )
        return iter->second.second;
    else
        return -1;//IF THERE IS AN ERROR
}