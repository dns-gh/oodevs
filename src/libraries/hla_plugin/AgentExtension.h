// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentExtension_h_
#define __AgentExtension_h_

#include "HlaExtension_ABC.h"

namespace dispatcher
{
    class Agent;
}

namespace hla
{
    class FederateFacade;

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentExtension : public HlaExtension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentExtension( dispatcher::Agent& holder, FederateFacade& federate );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize  ( UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentExtension( const AgentExtension& );            //!< Copy constructor
    AgentExtension& operator=( const AgentExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    //@}
};

}

#endif // __AgentExtension_h_
