// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticLinks_h_
#define __LogisticLinks_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Agent;

// =============================================================================
/** @class  LogisticLinks
    @brief  LogisticLinks
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLinks : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgChangeLiensLogistiquesAck >
                    , public Updatable_ABC< ASN1T_MsgAutomateCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinks( Controller& controller, const Resolver_ABC< Agent >& resolver );
    virtual ~LogisticLinks();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticLinks( const LogisticLinks& );            //!< Copy constructor
    LogisticLinks& operator=( const LogisticLinks& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgChangeLiensLogistiquesAck& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;

    Agent* tc2_;
    Agent* maintenanceSuperior_;
    Agent* medicalSuperior_;
    Agent* supplySuperior_;
    //@}
};

#endif // __LogisticLinks_h_
