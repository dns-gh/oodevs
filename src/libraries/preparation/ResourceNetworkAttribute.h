// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkAttribute_h_
#define __ResourceNetworkAttribute_h_

#include "Overridable_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver.h"

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class Controllers;
    class ResourceNetworkType;
}

namespace urban
{
    class ResourceNetworkAttribute;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ResourceNetworkAttribute
    @brief  ResourceNetworkAttribute
*/
// Created: JSR 2010-09-07
// =============================================================================
class ResourceNetworkAttribute : public kernel::ResourceNetwork_ABC
                               , public kernel::Serializable_ABC
                               , public Overridable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver );
             ResourceNetworkAttribute( kernel::Controllers& controllers, const urban::ResourceNetworkAttribute& network, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const;
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& ) const;
    virtual void SetOverriden( bool& overriden ) const;
    void Update( xml::xistream& xis );
    void Update( const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNode( xml::xistream& xis );
    void ReadLink( xml::xistream& xis, ResourceNode& node );
    void SetColor( const std::string& resource ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    unsigned int id_;
    const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver_;
    bool needSaving_;
    //@}
};

#endif // __ResourceNetworkAttribute_h_
