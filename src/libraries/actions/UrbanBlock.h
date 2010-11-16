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

namespace Common
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
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void CommitTo( Common::MsgMissionParameter_Value& message ) const;
    void CommitTo( Common::UrbanObjectKnowledgeId& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ThrowInvalidKnowledge() const;
    //@}
};

}
}

#endif // __ActionParameterUrbanKnowledge_h_
