// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_WorkingSession_h_
#define __crossbow_WorkingSession_h_

#include "WorkingSession_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Logger_ABC;
}

namespace tools
{
    class ExerciseConfig;
    class SessionConfig;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class QuerySessionData;

// =============================================================================
/** @class  WorkingSession
    @brief  WorkingSession
*/
// Created: JCR 2007-04-30
// =============================================================================
class WorkingSession : public WorkingSession_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            WorkingSession( Workspace_ABC& database, const tools::SessionConfig& config, const kernel::CoordinateConverter_ABC& converter, dispatcher::Logger_ABC& logger );
    virtual ~WorkingSession();
    //@}

    //! @name Get session id
    //@{
    int GetId() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WorkingSession( const WorkingSession& );            //!< Copy constructor
    WorkingSession& operator=( const WorkingSession& ); //!< Assignment operator
    //@}

private:
    //! @name Types Helpers
    //@{
    int GetExercise() const;

    void LoadExercise( const tools::ExerciseConfig& config, QuerySessionData& database );
    void LoadSession( const tools::SessionConfig& config, QuerySessionData& database );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, int >   T_Information;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_Information exercise_;
    T_Information session_;
    dispatcher::Logger_ABC& logger_;
    //@}
};

}
}

#endif // __crossbow_WorkingSession_h_
