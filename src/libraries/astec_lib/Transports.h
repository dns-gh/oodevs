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

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Agent_ABC;
class Controller;
class Displayer_ABC;
class DataDictionary;

// =============================================================================
/** @class  Transports
    @brief  Transports
*/
// Created: AGE 2006-02-13
// =============================================================================
class Transports : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Transports( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, DataDictionary& dico );
    virtual ~Transports();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Transports( const Transports& );            //!< Copy constructor
    Transports& operator=( const Transports& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent_ABC >& resolver_;
    const Agent_ABC* transporter_; 
    T_Agents transported_;
    //@}
};

#endif // __Transports_h_
