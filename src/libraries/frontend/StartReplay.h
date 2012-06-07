// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
             StartReplay( const tools::GeneralConfig& config,
                          const QString& exercise, const QString& session,
                          unsigned port, bool attach = false,
                          std::string commanderEndpoint = "" );
    virtual ~StartReplay();
    //@}

    //! @name operations
    //@{
    virtual void Start();
    //@}

private:
    //! @name data
    //@{
    std::auto_ptr< ConfigurationManipulator > configManipulator_;
    //@}

};

}

#endif // __StartReplay_h_
