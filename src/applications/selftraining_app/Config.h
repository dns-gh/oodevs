// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "frontend/Config.h"

enum Feature
{
    FEATURE_AUTHORING,
    FEATURE_TERRAIN_GENERATION,
    FEATURE_PREPARATION,
    FEATURE_RUNTIME,
    FEATURE_REPLAYER,
};

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: LDC 2008-10-27
// =============================================================================
class Config : public frontend::Config
{
public:
    //! @name Constructors/Destructor
    //@{
             Config();
    virtual ~Config();
    //@}

    //! @name Public Methods
    //@{
    bool CheckFeature( Feature feature ) const;
    bool HasFeature( Feature feature ) const;
    bool IsOnDebugMode() const;
    const tools::Path& GetExercise() const;
    const tools::Path& GetSession() const;
    const tools::Path& GetExportReplay() const;
    //@}

private:
    tools::Path exercise_;
    tools::Path session_;
    tools::Path exportReplay_;
};

#endif // __Config_h_
