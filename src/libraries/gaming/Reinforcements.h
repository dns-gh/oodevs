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
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
    class DataDictionary;
}

// =============================================================================
/** @class  Reinforcements
    @brief  Reinforcements
*/
// Created: AGE 2006-02-13
// =============================================================================
class Reinforcements : public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Reinforcements( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::DataDictionary& dico );
    virtual ~Reinforcements();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Reinforcements( const Reinforcements& );            //!< Copy constructor
    Reinforcements& operator=( const Reinforcements& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;

    const kernel::Agent_ABC* reinforced_;
    T_Agents reinforcements_;
    //@}
};

#endif // __Reinforcements_h_
