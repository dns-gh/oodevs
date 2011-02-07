// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterUrbanKnowledge_h_
#define __ActionParameterUrbanKnowledge_h_

#include "Knowledge_ABC.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class UrbanObjectKnowledgeId;
}

namespace kernel
{
    class EntityResolver_ABC;
    class UrbanKnowledgeConverter_ABC;
}

namespace actions {
namespace parameters {

// =============================================================================
/** @class  UrbanBlock
    @brief  UrbanBlock
// $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class UrbanBlock : public Knowledge_ABC< kernel::UrbanKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlock( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             UrbanBlock( const kernel::OrderParameter& parameter, unsigned long id, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             UrbanBlock( xml::xistream& xis, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             UrbanBlock( const kernel::OrderParameter& parameter, xml::xistream& xis, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~UrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual geometry::Point2f GetPosition() const;
    void CommitTo( sword::UrbanObjectKnowledgeId& message ) const;
    //@}
};

}
}

#endif // __ActionParameterUrbanKnowledge_h_
