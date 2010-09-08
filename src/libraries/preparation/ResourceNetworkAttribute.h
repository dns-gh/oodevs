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

#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class Controllers;
    class Displayer_ABC;
    class DotationType;
    class PropertiesDictionary;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationResolver/*, kernel::PropertiesDictionary& dico*/ );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( unsigned long resource, unsigned int i ) const;
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkAttribute( const ResourceNetworkAttribute& );            //!< Copy constructor
    ResourceNetworkAttribute& operator=( const ResourceNetworkAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadNode( xml::xistream& xis );
    void ReadLink( xml::xistream& xis, ResourceNode& node );
    void SetColor( unsigned long resource ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    unsigned int id_;
    const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver_;
    const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationResolver_;
    //@}
};

#endif // __ResourceNetworkAttribute_h_
