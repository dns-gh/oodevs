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
#include <boost/noncopyable.hpp>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  SpawnCommand
    @brief  Spawn command base class
            QProcess est buggué jusqu'à la moelle
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand : public Process_ABC
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    SpawnCommand( const tools::GeneralConfig& config, const char* exe, bool attach = false,
                  std::string commanderEndpoint = "", bool makeSilent = false  );
    SpawnCommand( const tools::GeneralConfig& config, unsigned long processId, bool attach = false,
                  std::string commanderEndpoint = "" );
    virtual ~SpawnCommand();
    //@}

    //! @name accessors
    //@{
    bool                 IsRunning() const;
    virtual void         Start();
    virtual bool         Wait();
    virtual void         Stop( bool forceProcessStop = true );
    virtual unsigned int GetPercentage() const;
    virtual QString      GetStatus() const;
    virtual std::string  GetStartedExercise() const;
    virtual std::string  GetExercise() const;
    virtual std::string  GetSession() const;
    void                 Attach( boost::shared_ptr< Process_ABC > process );
    void                 SetWorkingDirectory( const QString& directory );
    const std::string&   GetCommanderEndpoint() const;
    void                 SetCommanderEndpoint( const std::string& endpoint );
    //@}

protected:
    //! @name Operations
    //@{
    void AddRootDirArgument();
    void AddExerciseArgument( const QString& exercise );
    void AddSessionArgument ( const QString& session );
    void AddArgument( const QString& arg );
    //@}

private:
    //! @name Types
    //@{
    struct InternalData; //!< obscure data structure to hide OS implementation
    //@}

    //! @name Helpers
    //@{
    void StopProcess();
    void FillProcessInformation( unsigned long processId );
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
    bool                             forceProcessStop_;
    std::string                      workingDirectory_;
    bool                             stopped_;
    boost::shared_ptr< Process_ABC > attachment_;
    std::string                      networkCommanderEndpoint_;
    //@}
};

}

#endif // __SpawnCommand_h_
