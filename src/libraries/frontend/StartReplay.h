// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartReplay_h_
#define __StartReplay_h_

#include "SpawnCommand.h"

namespace frontend
{
    class ConfigurationManipulator;

// =============================================================================
/** @class  StartReplay
    @brief  StartReplay
*/
// Created: AGE 2007-10-05
// =============================================================================
class StartReplay : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             StartReplay( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const std::string& name );
    virtual ~StartReplay();
    //@}

    //! @name operations
    //@{
    virtual void Start();
    virtual void Stop();
    //@}

private:
    //! @name data
    //@{
    std::unique_ptr< ConfigurationManipulator > configManipulator_;
    //@}

};

}

#endif // __StartReplay_h_
