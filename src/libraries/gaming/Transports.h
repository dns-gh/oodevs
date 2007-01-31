// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Transports_h_
#define __Transports_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Transports
    @brief  Transports
*/
// Created: AGE 2006-02-13
// =============================================================================
class Transports : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Transports( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~Transports();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Transports( const Transports& );            //!< Copy constructor
    Transports& operator=( const Transports& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Agent_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::Agent_ABC* transporter_; 
    T_Agents transported_;
    //@}
};

#endif // __Transports_h_
