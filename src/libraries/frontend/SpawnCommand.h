// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SpawnCommand_h_
#define __SpawnCommand_h_

#include "Process_ABC.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    tools::Path MakeBinaryName( const tools::Path& prefix );

// =============================================================================
/** @class  SpawnCommand
    @brief  Spawn command base class
            QProcess est buggué jusqu'à la moelle
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand : public Process_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SpawnCommand( const tools::GeneralConfig& config, const tools::Path& exe, bool attach = false,
                  std::string commanderEndpoint = "", std::string jobName = ""  );
    virtual ~SpawnCommand();
    //@}

    //! @name accessors
    //@{
    int                  GetPid() const;
    bool                 IsRunning() const;
    virtual void         Start();
    virtual bool         Wait();
    virtual bool         Wait( const boost::posix_time::time_duration& duration ); ///< returns false on failures & timeouts
    virtual void         Stop();
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual tools::Path  GetStartedExercise() const;
    virtual tools::Path  GetExercise() const;
    virtual tools::Path  GetSession() const;
    void                 Attach( boost::shared_ptr< Process_ABC > process );
    void                 SetWorkingDirectory( const tools::Path& directory );
    const std::string&   GetCommanderEndpoint() const;
    //@}

protected:
    //! @name Operations
    //@{
    void AddRootDirArgument();
    void AddExerciseArgument( const tools::Path& exercise );
    void AddSessionArgument ( const tools::Path& session );
    void AddArgument( const QString& arg );
    virtual bool IsHidden() const;
    //@}

private:
    //! @name Types
    //@{
    struct InternalData; //!< obscure data structure to hide OS implementation
    //@}

    //! @name Helpers
    //@{
    void StopProcess();
    //@}

protected:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    //@}

private:
    //! @name Member data
    //@{
    QString                          commandLine_;
    std::auto_ptr<InternalData>      internal_; //!< obscure data structure to hide OS implementation
    bool                             attach_;   //!< if set to true , kill the attached process on exit
    tools::Path                      workingDirectory_;
    bool                             stopped_;
    boost::shared_ptr< Process_ABC > attachment_;
    std::string                      networkCommanderEndpoint_;
    std::string                      jobName_;
    //@}
};

}

#endif // __SpawnCommand_h_
