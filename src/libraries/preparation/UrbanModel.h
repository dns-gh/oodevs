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
#include "clients_gui/UrbanDisplayOptions.h"
#include <urban/Model.h>

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class ObjectTypes;
}

namespace gui
{
    class TerrainObjectProxy;
}

namespace tools
{
    class SchemaWriter_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public urban::Model
                 , public tools::Resolver< gui::TerrainObjectProxy >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, const StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& directoryPath, urban::WorldParameters& world, ::Model& model );
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void LoadUrbanState( xml::xistream& xis );
    void Purge();
    void SendCreation( urban::TerrainObject_ABC& urbanObject );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUrbanObject( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, gui::TerrainObjectProxy& proxy );
    template< typename T, typename U >
    void UpdateCapacity( xml::xistream& xis, gui::TerrainObjectProxy& proxy );
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
    std::string urbanStateVersion_;
    std::auto_ptr< gui::UrbanDisplayOptions > urbanDisplayOptions_;
    //@}
};

#endif // __preparation_UrbanModel_h_
