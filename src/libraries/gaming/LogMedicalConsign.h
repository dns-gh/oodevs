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

#include "LogisticsConsign_ABC.h"
#include "clients_kernel/Types.h"
#include <tools/Resolver_ABC.h>
#include <boost/function.hpp>

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class DisplayExtractor_ABC;
}

namespace sword
{
    class LogMedicalHandlingCreation;
    class LogMedicalHandlingUpdate;
    enum LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus;
}

class Simulation;

// =============================================================================
/** @class  LogMedicalConsign
    @brief  LogMedicalConsign
*/
// Created: AGE 2006-02-28
// =============================================================================
class LogMedicalConsign : public LogisticsConsign_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             LogMedicalConsign( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                                const Simulation& simulation, const sword::LogMedicalHandlingCreation& message,
                                kernel::Agent_ABC& consumer );
    virtual ~LogMedicalConsign();
    //@}

    //! @name Operations
    //@{
    bool Update( const sword::LogMedicalHandlingUpdate& message, kernel::Entity_ABC* handler );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool RefersToAgent( unsigned long id ) const;
    virtual bool RefersToAgent( const std::set< unsigned long >& id ) const;
    //@}

    //! @name Accessors
    //@{
    E_HumanRank GetRank() const;
    virtual kernel::Agent_ABC* GetConsumer() const;
    virtual kernel::Entity_ABC* GetHandler() const;
    bool IsMental() const;
    bool IsContamined() const;
    bool IsDiagnosed() const;
    E_HumanWound GetWound() const;
    sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus GetStatus() const;
    virtual QString GetStatusDisplay() const;
    virtual QString GetStatusDisplay( int status ) const;
    virtual QString GetCurrentStartedTime() const;
    virtual E_LogisticChain GetType() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    kernel::Agent_ABC& consumer_;
    kernel::Entity_ABC* pPionLogHandling_;
    E_HumanRank rank_;
    bool    bMentalDeceased_;
    bool    bContaminated_;
    bool    diagnosed_;
    E_HumanWound wound_;
    sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus nState_;
    //@}
};

#endif // __LogMedicalConsign_h_
