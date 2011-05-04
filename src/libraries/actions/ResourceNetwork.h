// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameterResourceNetwork_h_
#define __ActionParameterResourceNetwork_h_

#include "Entity.h"
#include "clients_kernel/Object_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace sword
{
    class ResourceNetworkElement;
}

namespace actions
{
    namespace parameters {

// =============================================================================
/** @class  ResourceNetwork
    @brief  ResourceNetwork
*/
// Created: JSR 2011-05-02
// =============================================================================
class ResourceNetwork : public Entity< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetwork( const kernel::OrderParameter& parameter, const kernel::Object_ABC& object, const std::string& resource, kernel::Controller& controller );
             ResourceNetwork( const kernel::OrderParameter& parameter, const sword::ResourceNetworkElement& resourceNetwork, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
             ResourceNetwork( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual geometry::Point2f GetPosition() const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddResourceParameter( const std::string& resource );
    void ReadParameter( xml::xistream& xis );
    void CommitTo( sword::ResourceNetworkElement& resourceNetwork ) const;
    //@}
};

    }

}

#endif // __ActionParameterResourceNetwork_h_
