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
            QProcess est buggué jusqu'à la moelle
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand 
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
    virtual bool Wait() ; 
    virtual void Start();
    virtual unsigned int GetPercentage() ; 
    //@}

protected:
    //! @name Operations
    //@{
    void Stop(); 
    void AddRootDirArgument();
    void AddExerciseArgument( const QString& exercise );
    void AddSessionArgument ( const QString& session );
    void addArgument( QString arg );
    //@}

    //! @name Member data 
    //@{
    const tools::GeneralConfig&     config_;
    //@}


private:
    //! @name Copy/Assignment
    //@{
    SpawnCommand( const SpawnCommand& );            //!< Copy constructor
    SpawnCommand& operator=( const SpawnCommand& ); //!< Assignment operator
    //@}

private:
    
    //! @name forward definition
    //@{
    struct InternalData ;    //!< obscure data structure to hide OS implementation 
    //@}

    //! @name Member data
    //@{
    QString                         commandLine_;
    std::auto_ptr<InternalData>     internal_ ;         //!< obscure data structure to hide OS implementation
    bool                            attach_ ;           //!< if set to true , kill the attached process on exit 
    //@}
};

}

#endif // __SpawnCommand_h_
