// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Frames_h_
#define __Frames_h_

namespace dispatcher
{

// =============================================================================
/** @class  Frame
    @brief  Frame
*/
// Created: AGE 2007-04-11
// =============================================================================
class Frame
{

public:
    //! @name Constructors/Destructor
    //@{
    Frame();
    //@}

    //! @name Operations
    //@{
    void Reset( unsigned offset = 0 );
    //@}

    //! @name Member data
    //@{
    unsigned offset_, size_;
    //@}
};

// =============================================================================
/** @class  KeyFrame
    @brief  KeyFrame
*/
// Created: AGE 2007-04-11
// =============================================================================
class KeyFrame
{
    public:
    //! @name Constructors/Destructor
    //@{
    KeyFrame();
    //@}

    //! @name Member data
    //@{
    unsigned offset_, size_;
    unsigned frameNumber_;
    //@}
};

}

#endif // __Frames_h_
