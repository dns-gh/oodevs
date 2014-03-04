// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
             Config( int argc, char** argv, const boost::shared_ptr< tools::RealFileLoaderObserver_ABC >& observer );
    virtual ~Config();
    //@}

    //! @name Accessor
    //@{
    bool HasGenerateScores() const;
    const tools::Path& GetFolderToMigrate() const;
    const tools::Path& GetQtNamesPath() const;
    //@}

private:
    //! @name Member data
    //@{
    bool generateScores_;
    tools::Path folderToMigrate_;
    tools::Path qtNamesPath_;
    //@}
};

#endif // __Config_h_
