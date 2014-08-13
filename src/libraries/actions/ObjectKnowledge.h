// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectKnowledge_h_
#define __ActionParameterObjectKnowledge_h_

#include "Entity.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

namespace sword
{
    class Id;
}

namespace kernel
{
    class EntityResolver_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ObjectKnowledge : public Entity< kernel::ObjectKnowledge_ABC >
                      , public kernel::KnowledgeListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ObjectKnowledge( const kernel::OrderParameter& parameter, uint32_t id, const kernel::ObjectKnowledgeConverter_ABC& converter,
                              const kernel::Entity_ABC* owner, kernel::Controller& controller, const kernel::EntityResolver_ABC& entities );
             ObjectKnowledge( const kernel::OrderParameter& parameter, const kernel::ObjectKnowledge_ABC* knowledge, kernel::Controller& controller );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( sword::Id& message ) const;
    unsigned int GetId() const;
    //@}

    //! @name Operations
    //@{
    virtual QString GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void NotifyDestruction();
    virtual bool IsSet() const;
    typedef const kernel::ObjectKnowledge_ABC* T_Concrete;
    virtual void SetValue( const T_Concrete& value );
    //@}

private:
    //! @name Helpers
    //@{
    typedef Entity< kernel::ObjectKnowledge_ABC > T_Entity;
    virtual std::string SerializeType() const;
    void Attach();
    //@}

    //! @name members
    //@{
    uint32_t id_;
    bool valid_;
    //@}
};

}
}

#endif // __ActionParameterObjectKnowledge_h_
