// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogMedicalConsign_h_
#define __LogMedicalConsign_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Displayer_ABC;
}

namespace MsgsSimToClient
{
    class MsgLogMedicalHandlingCreation;
    class MsgLogMedicalHandlingUpdate;
}

// =============================================================================
/** @class  LogMedicalConsign
    @brief  LogMedicalConsign
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogMedicalConsign : public kernel::Drawable_ABC
{
public:
    //! @name Operations
    //@{
             LogMedicalConsign( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const MsgsSimToClient::MsgLogMedicalHandlingCreation& message );
    virtual ~LogMedicalConsign();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgLogMedicalHandlingUpdate& message );
    void Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    unsigned int nID_;
    kernel::Agent_ABC& consumer_;
    kernel::Agent_ABC* pPionLogHandling_;
    bool    bMentalDeceased_;
    bool    bContaminated_;
    bool    diagnosed_;
    E_HumanWound wound_;
    E_LogMedicalHandlingStatus nState_;
    //@}
};

#endif // __LogMedicalConsign_h_
