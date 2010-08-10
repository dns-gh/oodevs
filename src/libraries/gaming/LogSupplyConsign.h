// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyConsign_h_
#define __LogSupplyConsign_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver.h"

class DotationRequest;

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controller;
    class Displayer_ABC;
    class DotationType;
}

namespace MsgsSimToClient
{
    class MsgLogSupplyHandlingCreation;
    class MsgLogSupplyHandlingUpdate;
}

// =============================================================================
/** @class  LogSupplyConsign
    @brief  LogSupplyConsign
*/
// Created: AGE 2006-02-21
// $$$$ AGE 2006-04-21: factorisations entre types de consignes
// =============================================================================
class LogSupplyConsign : public tools::Resolver< DotationRequest >
                       , public kernel::Drawable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogSupplyConsign( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver,
                               const tools::Resolver_ABC< kernel::Agent_ABC >&   agentResolver,
                               const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const MsgsSimToClient::MsgLogSupplyHandlingCreation& message );
    virtual ~LogSupplyConsign();
    //@}

    //! @name Accessors
    //@{
    void Update( const MsgsSimToClient::MsgLogSupplyHandlingUpdate& message );
    void Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
    const tools::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    const tools::Resolver_ABC< kernel::Agent_ABC >&   agentResolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    unsigned int nID_;
    kernel::Automat_ABC& consumer_;
    kernel::Automat_ABC* pAutomateLogHandling_;
    kernel::Agent_ABC*   pPionLogConvoying_;
    kernel::Automat_ABC* pAutomateLogProvidingConvoyResources_;
    E_LogSupplyHandlingStatus nState_;
    //@}
};

#endif // __LogSupplyConsign_h_
