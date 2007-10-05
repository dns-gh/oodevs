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

#include "clients_kernel/ExerciseConfig.h"
class Network;

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: NLD 2007-01-12
// =============================================================================
class Config : public kernel::ExerciseConfig
{

public:
    //! @name Constructors/Destructor
    //@{
             Config( int argc, char** argv );
    virtual ~Config();
    //@}

    //! @name Operations
    //@{
    void Connect( Network& network ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string host_;
    //@}
};

#endif // __Config_h_
