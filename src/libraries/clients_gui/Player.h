// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Player_h_
#define __Player_h_

#include "tools/thread/Processor_ABC.h"
#include "tools/win32/XPSLights.h"
#include "Melody.h"

// =============================================================================
/** @class  Player
    @brief  Player
*/
// Created: AGE 2006-11-13
// =============================================================================
class Player : public tools::thread::Processor_ABC< Melody >
{

public:
    //! @name Constructors/Destructor
    //@{
             Player();
    virtual ~Player();
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Process( const Melody& melody );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Player( const Player& );            //!< Copy constructor
    Player& operator=( const Player& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    XPSLights lights_;
    //@}
};

#endif // __Player_h_
