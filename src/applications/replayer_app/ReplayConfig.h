// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayConfig_h_
#define __ReplayConfig_h_

#include "dispatcher/Config.h"

// =============================================================================
/** @class  ReplayConfig
    @brief  ReplayConfig
*/
// Created: AGE 2007-10-04
// =============================================================================
class ReplayConfig : public dispatcher::Config
{

public:
    //! @name Constructors/Destructor
    //@{
             ReplayConfig();
    virtual ~ReplayConfig();
    //@}

    //! @name Operations
    //@{
    std::string GetRecord();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayConfig( const ReplayConfig& );            //!< Copy constructor
    ReplayConfig& operator=( const ReplayConfig& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string record_;
    //@}
};

#endif // __ReplayConfig_h_
