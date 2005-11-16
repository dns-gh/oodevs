// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/entities/Population.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:05 $
// $Revision: 24 $
// $Workfile: Population.h $
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"
#include "Testable_Entity.h"

namespace DIN
{
    class DIN_Input;
}

namespace TEST
{
    class Workspace;

// =============================================================================
// Created: SBO 2005-05-11
// =============================================================================
class Population : public Testable_Entity
{

public:
    //! @name Constructors/Destructor
    //@{
             Population( const Workspace& workspace, const ASN1T_MsgPopulationCreation& asnMsg );
    virtual ~Population();
    //@}

    //! @name Accessors
    //@{
    virtual       T_EntityId   GetId       () const;
	        const std::string& GetName     () const;
    //@}

    //! @name Messages handlers
    //@{
    //@}

    //! @name Operations
    //@{
    void SendMagicAction( int action ) const;
    //@}

    //! @name Mission Scheduling
    //@{
    void ScheduleAllMissions ( Scheduler& scheduler, uint nIteration = 1 );
    void ScheduleMission     ( Scheduler& scheduler, const std::string& strMissionName, uint nIteration = 1 );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId       nId_;
	std::string      strName_;
    //@}
};

} // end namespace TEST

#include "Population.inl"

#endif // __Population_h_
