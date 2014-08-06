// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PREPARATION_PREPACONFIG_H
#define PREPARATION_PREPACONFIG_H

#include "tools/ExerciseConfig.h"

namespace gui
{
    struct GamingCommonConfig;
}

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
class PrepaConfig : public tools::ExerciseConfig
{
public:
    //! @name Constructors/Destructor
    //@{
             PrepaConfig( int argc, char** argv, const boost::shared_ptr< tools::RealFileLoaderObserver_ABC >& observer );
    virtual ~PrepaConfig();
    //@}

    //! @name Accessor
    //@{
    bool HasGenerateScores() const;
    const tools::Path& GetFolderToMigrate() const;
    const tools::Path& GetQtNamesPath() const;
    bool HasMapnik() const;
    uint32_t GetMapnikThreads() const;
    //@}

private:
    //! @name Member data
    //@{
    bool generateScores_;
    tools::Path folderToMigrate_;
    tools::Path qtNamesPath_;
    std::unique_ptr< gui::GamingCommonConfig > common_;
    //@}
};

#endif // PREPARATION_PREPACONFIG_H
