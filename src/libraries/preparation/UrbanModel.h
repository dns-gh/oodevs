// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_UrbanModel_h_
#define __preparation_UrbanModel_h_

#include "tools/Resolver.h"
#include "clients_kernel/UrbanDisplayOptions.h"

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class ObjectTypes;
    class UrbanObject_ABC;
}

namespace tools
{
    class SchemaWriter_ABC;
}

class StaticModel;
class UrbanFactory_ABC;

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::UrbanObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, const StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void LoadUrban( xml::xistream& xis );
    void LoadUrbanState( xml::xistream& xis );
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadCity( xml::xistream& xis );
    void ReadDistrict( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadBlock( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadUrbanObject( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, kernel::UrbanObject_ABC& object );
    template< typename T, typename U >
    void UpdateCapacity( xml::xistream& xis, kernel::UrbanObject_ABC& object );
    void SerializeExercise( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void SerializeTerrain( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ObjectTypes& objectTypes_;
    const kernel::AccommodationTypes& accommodationTypes_;
    const tools::Resolver< kernel::Object_ABC >& objects_;
    std::auto_ptr< kernel::UrbanDisplayOptions > urbanDisplayOptions_;
    std::auto_ptr< UrbanFactory_ABC > factory_;
    //@}
};

#endif // __preparation_UrbanModel_h_
