// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SpawnCommand_h_
#define __SpawnCommand_h_

#include "Process_ABC.h"
#include <qprocess.h>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

// =============================================================================
/** @class  SpawnCommand
    @brief  Spawn command base class
            QProcess est buggu� jusqu'� la moelle
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand : public Process_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SpawnCommand( const tools::GeneralConfig& config, const char* exe, bool attach = false  );
    virtual ~SpawnCommand();
    //@}

    //! @name accessors
    //@{
    bool IsRunning() const;
    virtual bool Wait();
    virtual void Start();
    virtual unsigned int GetPercentage() const;
    virtual QString GetStatus() const;
    virtual std::string GetStartedExercise() const;
    //@}

protected:
    //! @name Operations
    //@{
    void Stop();
    void AddRootDirArgument();
    void AddExerciseArgument( const QString& exercise );
    void AddSessionArgument ( const QString& session );
    void addArgument( QString arg );
    void SetWorkingDirectory( const QString& directory );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SpawnCommand( const SpawnCommand& );            //!< Copy constructor
    SpawnCommand& operator=( const SpawnCommand& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct InternalData; //!< obscure data structure to hide OS implementation
    //@}

    //! @name Helpers
    //@{
    void CloseWindows();
    //@}

protected:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    //@}

private:
    //! @name Member data
    //@{
    QString                     commandLine_;
    std::auto_ptr<InternalData> internal_; //!< obscure data structure to hide OS implementation
    bool                        attach_;   //!< if set to true , kill the attached process on exit
    std::string                 workingDirectory_;
    //@}
};

}

#endif // __SpawnCommand_h_
