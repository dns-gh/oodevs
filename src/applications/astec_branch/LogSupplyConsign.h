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

#include "Types.h"
#include "ASN_Types.h"
#include "Resolver.h"
#include "Drawable_ABC.h"

class DotationRequest;
class Agent;
class Controller;
class Displayer_ABC;
class DotationType ;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class LogSupplyConsign : public Resolver< DotationRequest >, public Drawable_ABC
{

public:
    //! @name Constructor / Destructor
    //@{
             LogSupplyConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const Resolver_ABC< DotationType >& dotationResolver, const ASN1T_MsgLogRavitaillementTraitementCreation& asn );
    virtual ~LogSupplyConsign();
    //@}

    //! @name Accessors
    //@{
    void Display( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
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
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;
    const Resolver_ABC< DotationType >& dotationResolver_;

    uint   nID_;
    Agent& pion_;
    Agent* pAutomateLogHandling_;
    Agent* pPionLogConvoying_;
    Agent* pAutomateLogProvidingConvoyResources_;
    E_LogRavitaillementTraitementEtat nState_;
    //@}
};

#endif // __LogSupplyConsign_h_
