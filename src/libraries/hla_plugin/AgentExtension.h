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
#include "dispatcher/Updatable_ABC.h"
#include "game_asn/asn.h"

namespace dispatcher
{
    class Agent;
}

namespace hla
{

// =============================================================================
/** @class  AgentExtension
    @brief  AgentExtension
*/
// Created: SBO 2008-02-18
// =============================================================================
class AgentExtension : public HlaExtension_ABC
                     , public dispatcher::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentExtension( dispatcher::Agent& holder );
    virtual ~AgentExtension();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
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
