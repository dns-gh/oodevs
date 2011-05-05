// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "edxlhave_app_pch.h"
#include "Model.h"
#include "SwordClient.h"
#include "Hospital.h"
#include "MedicalTreatmentType.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace edxl;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
Model::Model( const boost::program_options::variables_map& options )
{
    ReadMedicalTreatments( options );
}
// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
Model::~Model()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::ReadMedicalThreatmentFilePath
// Created: BCI 2011-05-04
// -----------------------------------------------------------------------------
std::string Model::ReadMedicalThreatmentFilePath( const boost::program_options::variables_map& options )
{
    std::string physicalFilePath = ReadPhysicalFilePath( options );
    xml::xifstream xis( physicalFilePath );
    std::string medicalTreatmentFile;
    xis >> xml::start( "physical" )
        >> xml::start( "medical-treatment" )
        >> xml::attribute( "file", medicalTreatmentFile )
        >> xml::end
        >> xml::end;
    boost::filesystem::path dir = boost::filesystem::path( physicalFilePath ).parent_path();
    return ( dir / medicalTreatmentFile ).string();
}

// -----------------------------------------------------------------------------
// Name: Model::ReadPhysicalFilePath
// Created: BCI 2011-05-04
// -----------------------------------------------------------------------------
std::string Model::ReadPhysicalFilePath( const boost::program_options::variables_map& options )
{
    boost::filesystem::path rootDir( options[ "sword.root" ].as< std::string >() );
    xml::xifstream xis( ( rootDir / "exercises" / options[ "sword.exercise" ].as< std::string >() / "exercise.xml" ).string() );
    std::string dataset, physical;
    xis >> xml::start( "exercise" )
            >> xml::start( "model" )
                >> xml::attribute( "dataset", dataset )
                >> xml::attribute( "physical", physical )
            >> xml::end
        >> xml::end;

    return ( rootDir / "data/models" / dataset / "physical"  / physical / "physical.xml" ).string();
}

// -----------------------------------------------------------------------------
// Name: Model::ReadMedicalTreatment
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
void Model::ReadMedicalTreatments( const boost::program_options::variables_map& options )
{
    xml::xifstream xis( ReadMedicalThreatmentFilePath( options ) );
    xis >> xml::start( "medical-treatments" )
        >> xml::list( "medical-treatment", boost::bind( &Model::ReadMedicalTreatment, this, _1 ) )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Model::ReadMedicalTreatment
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
void Model::ReadMedicalTreatment( xml::xistream& xis )
{
    medicalTreatmentTypes_.push_back( new MedicalTreatmentType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: Model::Receive
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
void Model::Receive( const sword::ObjectCreation& message )
{
    objectNames_[ message.object().id() ] = message.name();
}

// -----------------------------------------------------------------------------
// Name: Model::Receive
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const Hospital* Model::Receive( const sword::ObjectUpdate& message )
{
    if( message.attributes().has_medical_treatment() )
    {
        std::string refId = message.attributes().medical_treatment().external_reference_id();
        if( const Hospital* hospital = FindHospital( refId ) )
            return hospital;
        else
        {
            Hospital* newHospital = new Hospital( message.object().id(), objectNames_[ message.object().id() ] );
            hospitals_.insert( refId, newHospital );
            return newHospital;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::FindHospital
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const Hospital* Model::FindHospital( const std::string& refId ) const
{
    try
    {
        return &hospitals_.at( refId );
    } catch( boost::bad_ptr_container_operation& )
    {
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::FindMedicalTreatmentType
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const MedicalTreatmentType* Model::FindMedicalTreatmentType( const std::string& name ) const
{
    BOOST_FOREACH( const MedicalTreatmentType& type, medicalTreatmentTypes_ )
    {
        if( type.GetName() == name )
            return &type;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Model::FindMedicalTreatmentType
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const MedicalTreatmentType* Model::FindMedicalTreatmentType( unsigned int id ) const
{
    BOOST_FOREACH( const MedicalTreatmentType& type, medicalTreatmentTypes_ )
    {
        if( type.GetId() == id )
            return &type;
    }
    return 0;
}
