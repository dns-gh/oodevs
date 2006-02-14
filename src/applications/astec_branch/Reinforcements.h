// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Reinforcements_h_
#define __Reinforcements_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Agent;
class Controller;

// =============================================================================
/** @class  Reinforcements
    @brief  Reinforcements
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reinforcements : public Extension_ABC
                     , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Reinforcements( Controller& controller, const Resolver_ABC< Agent >& resolver );
    virtual ~Reinforcements();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Reinforcements( const Reinforcements& );            //!< Copy constructor
    Reinforcements& operator=( const Reinforcements& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgUnitAttributes& message );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent* > T_Agents;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;

    const Agent* reinforced_;
    T_Agents reinforcements_;
    //@}
};

#endif // __Reinforcements_h_
