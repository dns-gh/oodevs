// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_MedicalTreatmentType.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "MIL_Time_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

MIL_MedicalTreatmentType::T_MedicalTreatmentTypeMap MIL_MedicalTreatmentType::types_;

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::ReadClass
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::ReadMedicalTreatment( xml::xistream& xis, const MIL_Time_ABC& time, std::set< unsigned int >& ids )
{
    const std::string name = xml::attribute< std::string >( xis, "name" );

    const MIL_MedicalTreatmentType*& pType = types_[ name ];
    if( pType )
        throw std::runtime_error( "Medical Treatment of Type " + name + " already exists" );
    pType = new MIL_MedicalTreatmentType( name, xis, time );
    if( ! ids.insert( pType->GetID() ).second )
        throw std::runtime_error( "Medical treatment type id of " + pType->GetName() + " already exists" );
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Initialize
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::Initialize( xml::xistream& xis, const MIL_Time_ABC& time )
{
    std::set< unsigned int > ids;
    MT_LOG_INFO_MSG( "Initializing Medical Treatment Types" );

    xis >> xml::start( "medical-treatments" )
            >> xml::list( "medical-treatment", boost::bind( &MIL_MedicalTreatmentType::ReadMedicalTreatment, _1, boost::cref( time ), boost::ref( ids ) ) )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicMedicalTreatmentType constructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::MIL_MedicalTreatmentType( const std::string& name, xml::xistream& xis, const MIL_Time_ABC& time )
    : name_            ( name )
    , treatments_      ( static_cast< int >( eDead ) + 1 )
    , deathThreshold_  ( -1 )
    , time_            ( time )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::optional >> xml::attribute( "death-threshold", deathThreshold_ )
        >> xml::optional
            >> xml::start( "injuries" )
                >> xml::list( "injury", *this, &MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect )
            >> xml::end;
}

namespace
{
    MIL_MedicalTreatmentType::E_InjuryCategories RetrieveInjuryCategory( const std::string& category )
    {
        if( category == "UA" )
            return MIL_MedicalTreatmentType::eUA;
        if( category == "UR" )
            return MIL_MedicalTreatmentType::eUR;
        if( category == "None" )
            return MIL_MedicalTreatmentType::eNone;
        if( category == "Dead" )
            return MIL_MedicalTreatmentType::eDead;
        throw std::runtime_error( "Unknown patient category: " + category );
    }
}

namespace
{
    float GetSimTimeValue( xml::xistream& xis, const std::string& /*tag*/, const MIL_Time_ABC& time )
    {
        double value = 0.;
        tools::ReadTimeAttribute( xis, "life-expectancy", value );
        return static_cast< float >( MIL_Tools::ConvertSecondsToSim( value, time ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::ReadMedicalTreatmentEffect( xml::xistream& xis )
{
    std::string category = xml::attribute< std::string >( xis, "category" );
    E_InjuryCategories patient = RetrieveInjuryCategory( category );

    ReadInjury( xis, treatments_[ static_cast< unsigned >( patient ) ] );
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::ReadInjury
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::ReadInjury( xml::xistream& xis, InjuryTreatment& injury )
{
    // ReadTimeAttribute already includes a the xml optional function
    injury.lifeExpectancy_ = GetSimTimeValue( xis, "life-expectancy", time_ );
    injury.treatmentTime_ = GetSimTimeValue( xis, "treatment-time", time_ );
    injury.hospitalisationTime_ = GetSimTimeValue( xis, "hospitalisation-time", time_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::Terminate
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
void MIL_MedicalTreatmentType::Terminate()
{
    for( CIT_MedicalTreatmentTypeMap it = types_.begin(); it != types_.end(); ++it )
        if ( it->second )
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
const MIL_MedicalTreatmentType* MIL_MedicalTreatmentType::Find( unsigned int nID )
{
    for( CIT_MedicalTreatmentTypeMap it = types_.begin(); it != types_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::RegisteredCount
// Created: JCR 2010-06-05
// -----------------------------------------------------------------------------
unsigned MIL_MedicalTreatmentType::RegisteredCount()
{
    return static_cast< unsigned >( types_.size() );
}

// -----------------------------------------------------------------------------
// Name: MIL_DynmaicMedicalTreatmentTypeA destructor
// Created: RFT 24/04/2008
// Modified: RFT 14/05/2008
// -----------------------------------------------------------------------------
MIL_MedicalTreatmentType::~MIL_MedicalTreatmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetID
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_MedicalTreatmentType::GetID() const
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
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetDeathThreshold
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_MedicalTreatmentType::GetDeathThreshold() const
{
    return deathThreshold_;
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetTreatmentTime
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
float MIL_MedicalTreatmentType::GetTreatmentTime( int injuryCategory ) const
{
    if ( treatments_.size() > ( unsigned int )injuryCategory )
        return treatments_[ injuryCategory ].treatmentTime_;
    else
        throw std::runtime_error( __FUNCTION__ + std::string( "Unknown injury category" ) );//IF THERE IS AN ERROR
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetHospitalisationTime
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
float MIL_MedicalTreatmentType::GetHospitalisationTime( int injuryCategory ) const
{
    if ( treatments_.size() > ( unsigned int )injuryCategory )
        return treatments_[ injuryCategory ].hospitalisationTime_;
    else
        throw std::runtime_error( __FUNCTION__ + std::string( "Unknown injury category" ) );//IF THERE IS AN ERROR
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetLifeExpectancy
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
float MIL_MedicalTreatmentType::GetLifeExpectancy( E_InjuryCategories injuryCategory ) const
{
    if ( treatments_.size() > ( unsigned int )injuryCategory )
        return treatments_[ injuryCategory ].lifeExpectancy_;
    else
        throw std::runtime_error( __FUNCTION__ + std::string( "Unknown injury category" ) );//IF THERE IS AN ERROR
}

// -----------------------------------------------------------------------------
// Name: MIL_MedicalTreatmentType::GetHospitalisationTime
// Created: RFT 19/05/2008
// Modified: none
// -----------------------------------------------------------------------------
unsigned int MIL_MedicalTreatmentType::GetInjuryThreshold( E_InjuryCategories injuryCategory ) const
{
    if ( treatments_.size() > ( unsigned int )injuryCategory )
        return treatments_[ injuryCategory ].injuryThreshold_;
    else
        throw std::runtime_error( __FUNCTION__ + std::string( "Unknown injury category" ) );//IF THERE IS AN ERROR
}