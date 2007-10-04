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

// =============================================================================
/** @class  SpawnCommand
    @brief  Spawn command base class
*/
// Created: AGE 2007-10-04
// =============================================================================
class SpawnCommand : public QProcess
{

public:
    //! @name Constructors/Destructor
    //@{
             SpawnCommand( QObject* parent, const tools::GeneralConfig& config, const char* exe );
    virtual ~SpawnCommand();
    //@}

protected:
    //! @name Operations
    //@{
    void AddRootDirArgument();
    void AddExerciseArgument( const std::string& exercise );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SpawnCommand( const SpawnCommand& );            //!< Copy constructor
    SpawnCommand& operator=( const SpawnCommand& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    //@}
};

#endif // __SpawnCommand_h_
