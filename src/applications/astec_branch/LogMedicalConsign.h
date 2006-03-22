// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogMedicalConsign_h_
#define __LogMedicalConsign_h_

#include "Types.h"
#include "ASN_Types.h"
#include "Resolver_ABC.h"

class Agent;
class Controller;
class Displayer_ABC;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMedicalConsign
{
public:
    //! @name Operations
    //@{
             LogMedicalConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const ASN1T_MsgLogSanteTraitementHumainCreation& asn );
    virtual ~LogMedicalConsign();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

    //! @name Network events
    //@{
    void Update( const ASN1T_MsgLogSanteTraitementHumainUpdate& message );
    //@}

    //! @name Accessors
    //@{
    const char* GetStateString() const;
    const char* GetWoundString() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogMedicalConsign( const LogMedicalConsign& );
    LogMedicalConsign& operator=( const LogMedicalConsign& );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;

    uint    nID_;
    Agent&  pion_;
    Agent*  pPionLogHandling_;
    bool    bMentalDeceased_;
    bool    bContaminated_;
    bool    diagnosed_;
    E_HumanWound wound_;
    E_LogSanteTraitementEtat nState_;
    //@}
};

#endif // __LogMedicalConsign_h_
