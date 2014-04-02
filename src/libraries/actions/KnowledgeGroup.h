// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "Entity.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: JSR 2010-04-19
// =============================================================================
class KnowledgeGroup : public Entity< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             KnowledgeGroup( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
             KnowledgeGroup( const kernel::OrderParameter& parameter, const kernel::KnowledgeGroup_ABC& group, kernel::Controller& controller );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::KnowledgeGroup_ABC >::CommitTo;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __KnowledgeGroup_h_
