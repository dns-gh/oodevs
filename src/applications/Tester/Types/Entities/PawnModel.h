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

#ifndef __PawnModel_h_
#define __PawnModel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Actions/Missions/Mission_Pawn_Type.h"

namespace TEST {

    class Scheduler;
    class Pawn;

// =============================================================================
/** @class  PawnModel
    @brief  PawnModel
    @par    Using example
    @code
    PawnModel;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class PawnModel
{

public:
    //! @name Constructors/Destructor
    //@{
             PawnModel( const std::string& strName, XmlInputArchive& archive );
    virtual ~PawnModel();
    //@}

    //! @name Accessors
    //@{
    void ScheduleAllMissions( Pawn& pawn, Scheduler& scheduler ) const;
    void ScheduleMission    ( Pawn& pawn, Scheduler& scheduler, const std::string& strMissionName ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PawnModel( const PawnModel& );            //!< Copy constructor
    PawnModel& operator=( const PawnModel& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const Mission_Pawn_Type* > T_MissionPawnTypeVector;
    typedef T_MissionPawnTypeVector::const_iterator CIT_MissionPawnTypeVector;
    //@}

private:
    //! @name Member data
    //@{
    std::string             strName_;
    T_MissionPawnTypeVector missionTypes_;
    //@}
};

} // end namespace TEST

#include "PawnModel.inl"

#endif // __PawnModel_h_
