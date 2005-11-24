// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __PopulationModel_h_
#define __PopulationModel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Actions/Missions/Mission_Population_Type.h"

namespace TEST {

    class Scheduler;
    class Population;

// =============================================================================
// Created: SBO 2005-08-11
// =============================================================================
class PopulationModel
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationModel( const std::string& strName, XmlInputArchive& archive );
    virtual ~PopulationModel();
    //@}

    //! @name Accessors
    //@{
    void ScheduleAllMissions( Population& population, Scheduler& scheduler, uint nIteration = 1 ) const;
    void ScheduleMission    ( Population& population, Scheduler& scheduler, const std::string& strMissionName, uint nIteration = 1 ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationModel( const PopulationModel& );            //!< Copy constructor
    PopulationModel& operator=( const PopulationModel& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const Mission_Population_Type* > T_MissionPopulationTypeVector;
    typedef T_MissionPopulationTypeVector::const_iterator CIT_MissionPopulationTypeVector;
    //@}

private:
    //! @name Member data
    //@{
    std::string                   strName_;
    T_MissionPopulationTypeVector missionTypes_;
    //@}
};

} // end namespace TEST

#endif // __PopulationModel_h_
