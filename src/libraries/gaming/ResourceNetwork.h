// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetwork_h_
#define __ResourceNetwork_h_

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "tools/Resolver.h"

namespace sword
{
    class ObjectAttributeResourceNetwork;
    class ResourceNetwork;
}

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class ResourceNetworkType;
    class PropertiesDictionary;
    class Object_ABC;
}

namespace sword
{
    class ObjectUpdate;
    class UrbanUpdate;
    class UrbanAttributes_Infrastructures;
}

// =============================================================================
/** @class  ResourceNetwork
    @brief  ResourceNetwork
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetwork : public kernel::ResourceNetwork_ABC
                      , public kernel::Updatable_ABC< sword::ObjectUpdate >
                      , public kernel::Updatable_ABC< sword::UrbanUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork( kernel::Controllers& controllers, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const sword::UrbanAttributes_Infrastructures& msg, kernel::PropertiesDictionary& dico );
             ResourceNetwork( kernel::Controllers& controllers, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const sword::ObjectAttributeResourceNetwork& msg, kernel::PropertiesDictionary& dico );
    virtual ~ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetwork( const ResourceNetwork& );            //!< Copy constructor
    ResourceNetwork& operator=( const ResourceNetwork& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::UrbanUpdate& message );
    void UpdateNetwork( kernel::Entity_ABC* entity, const sword::ResourceNetwork& msg );
    void SetColor( const std::string& resource ) const;
    void UpdateStipple( int value ) const;
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    unsigned int id_;
    bool isUrban_;
    const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver_;
    const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver_;
    //@}
};

#endif // __ResourceNetwork_h_
