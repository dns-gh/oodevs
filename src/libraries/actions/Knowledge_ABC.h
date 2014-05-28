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

namespace sword
{
    class Id;
}

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
    virtual void Serialize( xml::xostream& xos ) const;
    virtual QString GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void NotifyDestruction();
    void CommitTo( sword::Id& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyValueSet();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    bool isEntityValid_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC constructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::Knowledge_ABC(const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< ConcreteEntity >( parameter, controller )
    , id_( 0u )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC constructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::Knowledge_ABC(const kernel::OrderParameter& parameter, const ConcreteEntity* entity, kernel::Controller& controller )
    : Entity< ConcreteEntity >( parameter, entity, controller )
    , id_( 0u )
{
    NotifyValueSet();
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC destructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::~Knowledge_ABC()
{
    if( isEntityValid_ && GetValue() )
        const_cast< ConcreteEntity* >( GetValue() )->RemoveListener( *this );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
QString Knowledge_ABC< ConcreteEntity >::GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const
{
    if( !IsSet() || isEntityValid_ )
        return Entity< ConcreteEntity >::GetDisplayName( extractor );
    return extractor.GetDisplayName( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Display
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::Display( kernel::Displayer_ABC& displayer ) const
{
    if( !IsSet() || isEntityValid_ )
        Entity< ConcreteEntity >::Display( displayer );
    else
        displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::NotifyDestruction
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::NotifyDestruction()
{
    isEntityValid_ = false;
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Serialize
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    Parameter< const ConcreteEntity* >::Serialize( xos );
    if( id_ != 0 )
        xos << xml::attribute( "value", id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::NotifyValueSet
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::NotifyValueSet()
{
    if( !GetValue() )
        isEntityValid_ = false;
    else
    {
        const_cast< ConcreteEntity* >( GetValue() )->AddListener( *this );
        id_ = GetValue()->GetEntityId();
        isEntityValid_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::CommitTo
// Created: BAX 2013-10-31
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::CommitTo( sword::Id& message ) const
{
    if( GetValue() )
        message.set_id( id_ );
}

}
}

#endif // __ActionParameterKnowledge_ABC_h_
