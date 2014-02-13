// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __action_ResourceNetworkNode_h_
#define __action_ResourceNetworkNode_h_

#include "Entity.h"
#include "clients_kernel/Entity_ABC.h"

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
/** @class  ResourceNetworkNode
    @brief  ResourceNetworkNode
*/
// Created: ABR 2012-02-15
// =============================================================================
class ResourceNetworkNode : public Entity< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    ResourceNetworkNode( const kernel::OrderParameter& parameter, kernel::Controller& controller );
    ResourceNetworkNode( const kernel::OrderParameter& parameter, const kernel::Entity_ABC& object, const std::string& resource, kernel::Controller& controller );
    ResourceNetworkNode( const kernel::OrderParameter& parameter, const sword::ResourceNetworkElement& resourceNetwork, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~ResourceNetworkNode();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const;
    virtual geometry::Point2f GetPosition() const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    void AddResourceParameter( const std::string& resource );
    void CommitTo( sword::ResourceNetworkElement& resourceNetwork ) const;
    //@}
};

}
}

#endif // __action_ResourceNetworkNode_h_
