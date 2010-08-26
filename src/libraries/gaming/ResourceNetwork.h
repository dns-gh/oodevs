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
#include "tools/Resolver_ABC.h"

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class PropertiesDictionary;
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
class ResourceNetwork : public kernel::ResourceNetwork_ABC
                      , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork( kernel::Controllers& controllers, unsigned int id, bool isUrban, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg, kernel::PropertiesDictionary& dico );
    virtual ~ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& message );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QString GetLinkName( E_ResourceType type, unsigned int i ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetwork( const ResourceNetwork& );            //!< Copy constructor
    ResourceNetwork& operator=( const ResourceNetwork& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetColor( E_ResourceType type ) const;
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
    //@}
};

#endif // __ResourceNetwork_h_
