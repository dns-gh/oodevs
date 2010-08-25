// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_ReportUpdater_h_
#define __crossbow_ReportUpdater_h_

namespace dispatcher
{
    class Config;
    class Model_ABC;
}

namespace MsgsSimToClient
{
    class MsgReport;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class Workspace_ABC;
    class ReportFactory;
    class WorkingSession;

// =============================================================================
/** @class  ReportUpdater
    @brief  ReportUpdater
*/
// Created: JCR 2010-03-22
// =============================================================================
class ReportUpdater
{
public:
    //! @name Constructors/Destructor
    //@{
            ReportUpdater( Workspace_ABC& workspace, const dispatcher::Config& config, const dispatcher::Model_ABC& model, const WorkingSession& session );
    virtual ~ReportUpdater();
    //@}

    //! @name
    //@{
    void Clean();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgReport& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportUpdater( const ReportUpdater& );            //!< Copy constructor
    ReportUpdater& operator=( const ReportUpdater& ); //!< Assignment operator
    //@}

private:

    //! @name
    //@{
    std::auto_ptr< ReportFactory >  reportFactory_;
    //@}

    //! @name Member data
    //@{
    Database_ABC&           database_;
    const WorkingSession&   session_;
    //@}
};


}
}

#endif // __crossbow_ReportUpdater_h_
