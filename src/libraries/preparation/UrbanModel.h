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
#include <urban/Model.h>

namespace kernel
{
    class Controller;
}

namespace gui
{
    class TerrainObjectProxy;
}

namespace xml
{
    class xistream;
}

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
    explicit UrbanModel( kernel::Controller& controller ); 
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    bool Load( const std::string& directoryPath, urban::WorldParameters& world );
    void Serialize( const std::string& filename ) const;
    void LoadUrbanState( xml::xistream& xis );
    void Purge();
    void SendCreation( urban::TerrainObject_ABC& urbanObject );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadBlock( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, gui::TerrainObjectProxy& proxy );
    //@}

    //! @name Copy/Assignement
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __preparation_UrbanModel_h_
