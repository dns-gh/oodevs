// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef ADN_GeneralConfig_h
#define ADN_GeneralConfig_h

#include "tools/GeneralConfig.h"

// =============================================================================
/** @class  ADN_GeneralConfig
    @brief  ADN_GeneralConfig
*/
// Created: LGY 2013-05-16
// =============================================================================
class ADN_GeneralConfig : public tools::GeneralConfig
{
public:
    //! @name Constructors/Destructor
    //@{
            ADN_GeneralConfig( int argc, char** argv, const tools::Path& defaultRoot = "../" );
    virtual ~ADN_GeneralConfig();
    //@}

    //! @name Accessors
    //@{
    bool IsNoReadOnly() const;
    const tools::Path& GetInputFile() const;
    const tools::Path& GetOutputFile() const;
    const tools::Path& GetNewFile() const;
    const std::string& GetSwapLanguage() const;
    const tools::Path& GetQtNamesPath() const;
    //@}

private:
    //! @name Member data
    //@{
    bool noreadonly_;
    tools::Path inputFile_;
    tools::Path outputFile_;
    tools::Path newFile_;
    std::string swapLanguage_;
    tools::Path qtNamesPath_;
    //@}
};

#endif // ADN_GeneralConfig_h
