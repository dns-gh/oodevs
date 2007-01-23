// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GeneralConfig_h_
#define __GeneralConfig_h_

#include "CommandLineConfig_ABC.h"

namespace tools
{

// =============================================================================
/** @class  GeneralConfig
    @brief  GeneralConfig
*/
// Created: NLD 2007-01-10
// =============================================================================
class GeneralConfig : public CommandLineConfig_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GeneralConfig();
    virtual ~GeneralConfig();
    //@}

    //! @name Operations
    //@{
    std::string GetExerciseFile() const;
    std::string BuildExerciseChildFile( const std::string& file ) const;

    std::string GetPhysicalFile( const std::string& dataset, const std::string& physical ) const;
    std::string BuildPhysicalChildFile( const std::string& dataset, const std::string& physical, const std::string& file ) const;

    std::string GetDecisionalFile( const std::string& dataset ) const;
    std::string BuildDecisionalChildFile( const std::string& dataset, const std::string& file ) const;

    std::string GetTerrainFile( const std::string& terrain ) const;
    std::string BuildTerrainChildFile( const std::string& terrain, const std::string& file ) const;

    std::string GetGameFile() const;
    std::string BuildGameChildFile( const std::string& file ) const;

    virtual void Parse( int argc, char** argv );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GeneralConfig( const GeneralConfig& );            //!< Copy constructor
    GeneralConfig& operator=( const GeneralConfig& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadExerciseConfig();
    //@}

private:
    //! @name Member data
    //@{
    std::string terrainsPath_;
    std::string modelsPath_;
    std::string exercisesPath_;
    std::string gamesPath_;
    std::string exerciseName_;
    std::string gameName_;
    //@}
};

}

#endif // __GeneralConfig_h_
