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

namespace dispatcher
{
    class Config;
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
class WorkingSession
{
public:
    //! @name Constructors/Destructor
    //@{
            WorkingSession( Workspace_ABC& database, const dispatcher::Config& config );
    virtual ~WorkingSession();
    //@}

    //! @name Get session id
    //@{
    int GetId() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WorkingSession( const WorkingSession& );            //!< Copy constructor
    WorkingSession& operator=( const WorkingSession& ); //!< Assignement operator
    //@}

private:
    //! @name Types Helpers
    //@{
    int GetExercise() const;

    void LoadExercise( const dispatcher::Config& config, QuerySessionData& database );
    void LoadSession( const dispatcher::Config& config, QuerySessionData& database );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, int >   T_Information;
    //@}

private:
    //! @name Member data
    //@{
    T_Information exercise_;
    T_Information session_;
    //@}
};

}
}

#endif // __crossbow_WorkingSession_h_
