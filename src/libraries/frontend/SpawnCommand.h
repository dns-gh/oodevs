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
#include <memory>
#include <string>

namespace boost
{
namespace posix_time
{
    class time_duration;
}
}

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
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand : public Process_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SpawnCommand( const tools::GeneralConfig& config,
                  const tools::Path& exe,
                  const std::string& name );
    virtual ~SpawnCommand();
    //@}

    //! @name Process_ABC methods
    //@{
    virtual unsigned int       GetPercentage() const;
    virtual QString            GetStatus() const;
    virtual tools::Path        GetStartedExercise() const;
    virtual tools::Path        GetExercise() const;
    virtual tools::Path        GetSession() const;
    virtual const std::string& GetName() const;
    //@}

    //! @name Virtual methods
    //@{
    virtual void Start();
    virtual void Stop();
    //@}

    //! @name Methods
    //@{
    int  GetPid() const;
    // Returns true if cmd is finished before <duration>
    bool Wait( const boost::posix_time::time_duration& duration );
    void Attach( const boost::shared_ptr< Process_ABC >& process );
    void AddArgument( const std::string& arg );
    void AddArgument( const std::string& key, const std::string& value );
    void AddRootArgument();
    void AddExerciseArgument( const tools::Path& path );
    void AddSessionArgument( const tools::Path& path );
    void SetWorkingDirectory( const tools::Path& path );
    //@}

private:
    //! member data
    const tools::GeneralConfig& config_;
    const std::string name_;
    struct Private;
    std::unique_ptr< Private > private_;
    //@}
};

}

#endif // __SpawnCommand_h_
