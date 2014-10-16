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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include <tools/Resolver.h>

namespace google
{
    namespace protobuf
    {
        template< typename T > class RepeatedPtrField;
    }
}

namespace sword
{
    class ObjectAttributeResourceNetwork;
    class ResourceNetwork;
    class ObjectUpdate;
    class UrbanUpdate;
    class UrbanAttributes_Infrastructures;
}

namespace kernel
{
    class ResourceNetworkType;
    class Object_ABC;
    class Entity_ABC;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  ResourceNetwork
    @brief  ResourceNetwork
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetwork : public gui::ResourceNetwork_ABC
                      , public kernel::Updatable_ABC< sword::ObjectUpdate >
                      , public kernel::Updatable_ABC< sword::UrbanUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork( kernel::Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, gui::PropertiesDictionary* dico );
             ResourceNetwork( kernel::Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const sword::UrbanAttributes_Infrastructures& msg, gui::PropertiesDictionary* dico );
             ResourceNetwork( kernel::Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const sword::ObjectAttributeResourceNetwork& msg, gui::PropertiesDictionary* dico );
    virtual ~ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const gui::Viewport_ABC& viewport, const gui::GLView_ABC& tools, const geometry::Point2f& position, float alpha ) const;
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::UrbanUpdate& message );
    const kernel::Entity_ABC* FindEntity( unsigned int id ) const;
    void UpdateNetwork( kernel::Entity_ABC* entity, const sword::ResourceNetwork& msg );
    void SetColor( const std::string& resource, float alpha ) const;
    void UpdateStipple( int value ) const;
    void UpdateDictionary( const google::protobuf::RepeatedPtrField< sword::ResourceNetwork >& networks );
    geometry::Point2f GetPosition( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Entity_ABC& entity_;
    const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanResolver_;
    const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver_;
    gui::PropertiesDictionary* dico_;
    std::vector< std::string > dicoResources_;
    static int maxFlow_;
    static double stippleFactor_;
    //@}
};

#endif // __ResourceNetwork_h_
