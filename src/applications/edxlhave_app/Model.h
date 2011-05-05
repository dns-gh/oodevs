// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <boost/noncopyable.hpp>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace sword
{
    class ObjectCreation;
    class ObjectUpdate;
}

namespace xml
{
    class xistream;
}

namespace edxl
{
    class MedicalTreatmentType;
    class Hospital;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: BCI 2011-04-29
// =============================================================================
class Model : boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    Model( const boost::program_options::variables_map& options );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{

    void Receive( const sword::ObjectCreation& message );
    const Hospital* Receive( const sword::ObjectUpdate& message );
    const Hospital* FindHospital( const std::string& refId ) const;
    const MedicalTreatmentType* FindMedicalTreatmentType( const std::string& type ) const;
    const MedicalTreatmentType* FindMedicalTreatmentType( unsigned int id ) const;
    //@}

private:
    //! @name Helpers
    //@{
    std::string ReadMedicalThreatmentFilePath( const boost::program_options::variables_map& options );
    std::string ReadPhysicalFilePath( const boost::program_options::variables_map& options );
    void ReadMedicalTreatments( const boost::program_options::variables_map& options );
    void ReadMedicalTreatment( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, std::string > objectNames_;
    boost::ptr_map< std::string, Hospital > hospitals_;
    boost::ptr_vector< MedicalTreatmentType > medicalTreatmentTypes_;
    //@}
};

}//namespace edxl

#endif // __Model_h_
