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

#ifndef __AutomatModel_h_
#define __AutomatModel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

    class Mission_Automat_Type;
    class Automat;
    class Scheduler;

// =============================================================================
/** @class  AutomatModel
    @brief  AutomatModel
    @par    Using example
    @code
    AutomatModel;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class AutomatModel
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatModel( const std::string& strName, XmlInputArchive& archive );
    virtual ~AutomatModel();
    //@}

    //! @name Accessors
    //@{
    void ScheduleAllMissions( Automat& automat, Scheduler& scheduler, uint nIteration = 1 ) const;
    void ScheduleMission    ( Automat& automat, Scheduler& scheduler, const std::string& strMissionName, uint nIteration = 1 ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatModel( const AutomatModel& );            //!< Copy constructor
    AutomatModel& operator=( const AutomatModel& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const Mission_Automat_Type* > T_MissionAutomatTypeVector;
    typedef T_MissionAutomatTypeVector::const_iterator CIT_MissionAutomatTypeVector;
    //@}

private:
    //! @name Member data
    //@{
    std::string                strName_;
    T_MissionAutomatTypeVector missionTypes_;
    //@}
};

} // end namespace TEST

#endif // __AutomatModel_h_
