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
#include "game_asn/Asn.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogMedicalConsign : public kernel::Drawable_ABC
{
public:
    //! @name Operations
    //@{
             LogMedicalConsign( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const ASN1T_MsgLogSanteTraitementHumainCreation& asn );
    virtual ~LogMedicalConsign();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
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
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;

    uint    nID_;
    kernel::Agent_ABC&  pion_;
    kernel::Agent_ABC*  pPionLogHandling_;
    bool    bMentalDeceased_;
    bool    bContaminated_;
    bool    diagnosed_;
    E_HumanWound wound_;
    E_LogSanteTraitementEtat nState_;
    //@}
};

#endif // __LogMedicalConsign_h_
