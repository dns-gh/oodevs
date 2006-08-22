// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign.h $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: LogSupplyConsign.h $
//
// *****************************************************************************

#ifndef __LogSupplyConsign_h_
#define __LogSupplyConsign_h_

#include "clients_kernel/Types.h"
#include "ASN_Types.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Drawable_ABC.h"

class DotationRequest;

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
    class DotationType;
}

// $$$$ AGE 2006-04-21: factorisations entre types de consignes
// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogSupplyConsign : public kernel::Resolver< DotationRequest >
                       , public kernel::Drawable_ABC
{

public:
    //! @name Constructor / Destructor
    //@{
             LogSupplyConsign( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver, const ASN1T_MsgLogRavitaillementTraitementCreation& asn );
    virtual ~LogSupplyConsign();
    //@}

    //! @name Accessors
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Network events
    //@{
    void Update( const ASN1T_MsgLogRavitaillementTraitementUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogSupplyConsign( const LogSupplyConsign& );
    LogSupplyConsign& operator=( const LogSupplyConsign& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver_;

    uint   nID_;
    kernel::Agent_ABC& pion_;
    kernel::Agent_ABC* pAutomateLogHandling_;
    kernel::Agent_ABC* pPionLogConvoying_;
    kernel::Agent_ABC* pAutomateLogProvidingConvoyResources_;
    E_LogRavitaillementTraitementEtat nState_;
    //@}
};

#endif // __LogSupplyConsign_h_
