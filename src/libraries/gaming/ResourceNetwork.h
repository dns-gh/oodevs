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
#include "resource_network/Types.h"
#include "tools/Resolver_ABC.h"

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class Extension_ABC;
    class Object_ABC;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes_Infrastructures;
}

// =============================================================================
/** @class  ResourceNetwork
    @brief  ResourceNetwork
*/
// Created: JSR 2010-08-19
// =============================================================================
class ResourceNetwork : public kernel::Drawable_ABC
                      , public kernel::Extension_ABC
{
    // TODO structures temporaires, à cleaner/déplacer.
    struct ResourceLink
    {
        bool urban_;
        unsigned int id_;
        int capacity_;
    };

    struct ResourceNode
    {
        bool isProducer_;
        resource::EResourceType type_;
        std::vector< ResourceLink > links_;    
        bool isEnabled_;
        bool hasStock_;
        unsigned int stock_;
    };

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork( kernel::Controller& controller, unsigned int id, bool isUrban, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg );
    virtual ~ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& message );
    void Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetwork( const ResourceNetwork& );            //!< Copy constructor
    ResourceNetwork& operator=( const ResourceNetwork& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int id_;
    bool isUrban_;
    const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver_;
    const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    std::map< resource::EResourceType, ResourceNode > resourceNodes_;
    //@}
};

#endif // __ResourceNetwork_h_
