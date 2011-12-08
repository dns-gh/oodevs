// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartExercise_h_
#define __StartExercise_h_

#include "SpawnCommand.h"

namespace frontend
{
    class ConfigurationManipulator;

// =============================================================================
/** @class  StartExercise
    @brief  StartExercise
*/
// Created: AGE 2007-10-04
// =============================================================================
class StartExercise : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
    StartExercise(  const tools::GeneralConfig& config, const QString& exercise,
                    const QString& session, bool attach, bool launchDispatchedIfNotEmbedded = true,
                    std::string commanderEndpoint = "", bool makeSilent = false );
    StartExercise(  const tools::GeneralConfig& config, const QString& exercise,
                    const QString& session, const QString& checkpoint, bool attach, bool launchDispatchedIfNotEmbedded = true,
                    std::string commanderEndpoint = "", bool makeSilent = false );
    virtual ~StartExercise();
    //@}

    //! @name Operations
    //@{
    virtual void         Start();
    virtual unsigned int GetPercentage() const;
    virtual std::string  GetStartedExercise() const;
    virtual std::string  GetExercise() const;
    virtual std::string  GetSession() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartExercise( const StartExercise& );            //!< Copy constructor
    StartExercise& operator=( const StartExercise& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QString GetStatus() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string exercise_;
    std::string session_ ;
    std::auto_ptr< SpawnCommand > dispatcher_;
    std::auto_ptr< SpawnCommand > edxl_;
    std::auto_ptr< ConfigurationManipulator > configManipulator_;
    unsigned int percentage_;
    //@}
};

}

#endif // __StartExercise_h_
