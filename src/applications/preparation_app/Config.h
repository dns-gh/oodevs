// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "tools/ExerciseConfig.h"

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

// =============================================================================
/** @class  Config
    @brief  Config
    // $$$$ AGE 2008-03-13:  huh  ?
*/
// Created: NLD 2007-01-12
// =============================================================================
class Config : public tools::ExerciseConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv, tools::RealFileLoaderObserver_ABC& observer );
    virtual ~Config();
    //@}

    //! @name Accessor
    //@{
    bool HasGenerateScores() const;
    const tools::Path& GetFolderToMigrate();
    //@}

private:
    //! @name Member data
    //@{
    bool generateScores_;
    tools::Path folderToMigrate_;
    //@}
};

#endif // __Config_h_
