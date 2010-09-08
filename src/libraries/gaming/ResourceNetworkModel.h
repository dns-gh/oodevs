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

class Model;
class StaticModel;

namespace kernel
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
    kernel::ResourceNetworkSelectionObserver& observer_;
    //@}
};

template< typename T >
void ResourceNetworkModel::Create( kernel::Entity_ABC& entity, const T& msg )
{
    kernel::PropertiesDictionary& dico = entity.Get< kernel::PropertiesDictionary >();
    ResourceNetwork* element = new ResourceNetwork( controllers_, entity.GetId(), model_.urbanObjects_, model_.objects_, staticModel_.objectTypes_, msg, dico );
    entity.Attach< kernel::ResourceNetwork_ABC >( *element );
}

#endif // __ResourceNetworkModel_h_
