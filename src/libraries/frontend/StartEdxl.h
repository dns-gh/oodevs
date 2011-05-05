// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StartEdxl_h_
#define __StartEdxl_h_

#include "SpawnCommand.h"

namespace frontend
{
    class ConfigurationManipulator;

// =============================================================================
/** @class  StartEdxl
    @brief  StartEdxl
*/
// Created: AGE 2007-10-05
// =============================================================================
class StartEdxl : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             StartEdxl( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach );
    virtual ~StartEdxl();
    //@}

    //! @name operations
    //@{
    virtual void Start();
    //@}

private:
    //! @name data
    //@{
    std::auto_ptr< ConfigurationManipulator > configManipulator_;
    bool mustRun_;
    //@}

};

}

#endif // __StartEdxl_h_
