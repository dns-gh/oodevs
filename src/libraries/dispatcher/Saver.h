// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Saver_h_
#define __Saver_h_

#include "Frames.h"

namespace dispatcher
{
    class Savable_ABC;

// =============================================================================
/** @class  Saver
    @brief  Saver
*/
// Created: AGE 2007-04-10
// =============================================================================
class Saver
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Saver( const std::string& directory );
    virtual ~Saver();
    //@}

    //! @name Operations
    //@{
    void StartFrame( const Savable_ABC& message );
    void SaveUpdateMessage  ( const Savable_ABC& message );
    void EndFrame( const Savable_ABC& message );
    void SaveKeyFrame( const Savable_ABC& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Saver( const Saver& );            //!< Copy constructor
    Saver& operator=( const Saver& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Flush();
    //@}

private:
    //! @name Member data
    //@{
    Frame current_;
    std::ofstream index_;
    std::ofstream keyIndex_;
    std::ofstream key_;
    std::ofstream update_;
    unsigned frameCount_;
    //@}
};

}

#endif // __Saver_h_
