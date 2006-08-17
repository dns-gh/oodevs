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

#include "clients_kernel/Types.h"
#include "ASN_Types.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

class Agent_ABC;
class Controller;
class Displayer_ABC;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMedicalConsign : public Drawable_ABC
{
public:
    //! @name Operations
    //@{
             LogMedicalConsign( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const ASN1T_MsgLogSanteTraitementHumainCreation& asn );
    virtual ~LogMedicalConsign();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

    //! @name Network events
    //@{
    void Update( const ASN1T_MsgLogSanteTraitementHumainUpdate& message );
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
    const Resolver_ABC< Agent_ABC >& resolver_;

    uint    nID_;
    Agent_ABC&  pion_;
    Agent_ABC*  pPionLogHandling_;
    bool    bMentalDeceased_;
    bool    bContaminated_;
    bool    diagnosed_;
    E_HumanWound wound_;
    E_LogSanteTraitementEtat nState_;
    //@}
};

#endif // __LogMedicalConsign_h_
