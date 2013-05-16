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
    explicit ADN_GeneralConfig( const tools::Path& defaultRoot = "../" );
    virtual ~ADN_GeneralConfig();
    //@}

    //! @name Operations
    //@{
    virtual void Parse( int argc, char** argv );
    //@}

    //! @name Accessors
    //@{
    bool IsDevMode() const;
    //@}

private:
    //! @name Member data
    //@{
    bool devMode_;
    //@}
};

#endif // ADN_GeneralConfig_h
