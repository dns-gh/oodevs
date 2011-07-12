// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameterKnowledge_ABC_h_
#define __ActionParameterKnowledge_ABC_h_

#include "Entity.h"
#include "clients_kernel/KnowledgeListener_ABC.h"
#include "ParameterVisitor_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Knowledge_ABC
    @brief  Knowledge_ABC
*/
// Created: JSR 2010-05-20
// =============================================================================
template< typename ConcreteEntity >
class Knowledge_ABC : public Entity< ConcreteEntity >
                    , public kernel::KnowledgeListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Knowledge_ABC(const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Knowledge_ABC(const kernel::OrderParameter& parameter, const ConcreteEntity* entity, kernel::Controller& controller );
    virtual ~Knowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void NotifyDestruction();
    template< typename M >
    void CommitTo( M& message ) const
    {
        if( GetValue() )
            message.set_id( GetValue()->GetEntity()->GetId() );
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void NotifyValueSet();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    bool isEntityValid_;
    //@}
};

#include "Knowledge_ABC.inl"

    }
}

#endif // __ActionParameterKnowledge_ABC_h_
