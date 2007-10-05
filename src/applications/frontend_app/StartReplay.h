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
             StartReplay( QObject* parent, const tools::GeneralConfig& config, const QString& exercise, const QString& replay );
    virtual ~StartReplay();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartReplay( const StartReplay& );            //!< Copy constructor
    StartReplay& operator=( const StartReplay& ); //!< Assignment operator
    //@}
};

#endif // __StartReplay_h_
