// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkModel_h_
#define __ResourceNetworkModel_h_

#include "Model.h"
#include "ObjectsModel.h"
#include "StaticModel.h"
#include "UrbanModel.h"
#include "clients_kernel/ObjectTypes.h"
#include <tools/Resolver.h>

namespace gui
{
    class ResourceNetworkSelectionObserver;
}

// =============================================================================
/** @class  ResourceNetworkModel
    @brief  ResourceNetworkModel
*/
// Created: JSR 2010-08-18
// =============================================================================
class ResourceNetworkModel
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel );
    virtual ~ResourceNetworkModel();
    //@}

    //! @name Operations
    //@{
    void Create( kernel::Entity_ABC& entity );
    template< typename T >
    void Create( kernel::Entity_ABC& entity, const T& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkModel( const ResourceNetworkModel& );            //!< Copy constructor
    ResourceNetworkModel& operator=( const ResourceNetworkModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Model& model_;
    const StaticModel& staticModel_;
    std::unique_ptr< gui::ResourceNetworkSelectionObserver > observer_;
    //@}
};

template< typename T >
void ResourceNetworkModel::Create( kernel::Entity_ABC& entity, const T& msg )
{
    gui::PropertiesDictionary* dico = entity.Retrieve< gui::PropertiesDictionary >();
    ResourceNetwork* element = new ResourceNetwork( controllers_, entity, model_.urbanObjects_, model_.objects_, staticModel_.objectTypes_, msg, dico );
    entity.Attach< gui::ResourceNetwork_ABC >( *element );
}

#endif // __ResourceNetworkModel_h_
