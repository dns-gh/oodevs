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
    class Logger_ABC;
}

namespace sword
{
    class Report;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class Workspace_ABC;
    class ReportFactory;
    class WorkingSession_ABC;

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
            ReportUpdater( Workspace_ABC& workspace, const dispatcher::Config& config, const dispatcher::Model_ABC& model, const WorkingSession_ABC& session, dispatcher::Logger_ABC& logger );
    virtual ~ReportUpdater();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::Report& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportUpdater( const ReportUpdater& );            //!< Copy constructor
    ReportUpdater& operator=( const ReportUpdater& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void Clean( dispatcher::Logger_ABC& logger );
    //@}

private:

    //! @name
    //@{
    std::auto_ptr< ReportFactory >  reportFactory_;
    //@}

    //! @name Member data
    //@{
    Workspace_ABC&          workspace_;
    const WorkingSession_ABC&   session_;
    //@}
};


}
}

#endif // __crossbow_ReportUpdater_h_
