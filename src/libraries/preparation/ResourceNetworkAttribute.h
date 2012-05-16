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

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class ResourceNetworkType;
    class UrbanObject_ABC;
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
private:
    //! @name Types
    //@{
    typedef tools::Resolver_ABC< kernel::UrbanObject_ABC > T_Urbans;
    typedef tools::Resolver_ABC< kernel::Object_ABC > T_Objects;
    typedef tools::StringResolver< kernel::ResourceNetworkType > T_Resources;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, const geometry::Point2f position
                                     , const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources );
             ResourceNetworkAttribute( kernel::Controllers& controllers, const geometry::Point2f position
                                     , const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkName( const std::string& resource, unsigned int i ) const;
    virtual kernel::ResourceNetwork_ABC::ResourceNode& FindOrCreateResourceNode( std::string resource );
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
    const geometry::Point2f position_;
    const T_Urbans& urbans_;
    const T_Objects& objects_;
    const T_Resources& resources_;
    bool needSaving_;
    //@}
};

#endif // __ResourceNetworkAttribute_h_
