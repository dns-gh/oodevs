// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AggregateEntityClass_h
#define plugins_hla_AggregateEntityClass_h

#include "AgentListener_ABC.h"

namespace hla
{
    template< typename T > class Class;
    class Federate;
}

namespace plugins
{
namespace hla
{
    class HlaExtension_ABC;
    class AgentSubject_ABC;

// =============================================================================
/** @class  AggregateEntityClass
    @brief  Aggregate entity class
*/
// Created: AGE 2008-02-22
// =============================================================================
class AggregateEntityClass : public AgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntityClass( ::hla::Federate& federate, AgentSubject_ABC& subject );
    virtual ~AggregateEntityClass();
    //@}

    //! @name Operations
    //@{
    virtual void Created( dispatcher::Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    struct UnitRegistration;
    //@}

private:
    //! @name Member data
    //@{
    unsigned short id_;
    AgentSubject_ABC& subject_;
    std::auto_ptr< UnitRegistration > registration_;
    std::auto_ptr< ::hla::Class< HlaExtension_ABC > > hlaClass_;
    //@}
};

}
}

#endif // plugins_hla_AggregateEntityClass_h
