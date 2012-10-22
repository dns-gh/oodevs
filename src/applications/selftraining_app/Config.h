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

private:
    //! @name Copy/Assignment
    //@{
    Config( const Config& );            //!< Copy constructor
    Config& operator=( const Config& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    bool hasAuthoring_;
    bool hasTerrainGeneration_;
    bool hasPreparation_;
    bool hasRuntime_;
    bool hasReplayer_;
    //@}
};

#endif // __Config_h_
