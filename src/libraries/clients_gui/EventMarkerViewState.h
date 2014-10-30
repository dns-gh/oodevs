// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventMarkerViewState_h_
#define __EventMarkerViewState_h_

#include <string>
#include <tools/Path.h>

namespace gui
{

// =============================================================================
/** @class  EventMarkerViewState
    @brief  EventMarkerViewState
*/
// Created: JSR 2014-10-24
// =============================================================================
    struct EventMarkerViewState
{
    //! @name Constructors/Destructor
    //@{
    EventMarkerViewState()
        : resetDrawings_( false )
    {
        // NOTHING
    }

    EventMarkerViewState( const std::string& label, 
                          const std::string& description,
                          bool resetDrawings,
                          const std::string& drawingsPath,
                          const std::string& configurationPath )
        : label_( label )
        , description_( description )
        , resetDrawings_( resetDrawings )
        , drawingsPath_( tools::Path::FromUTF8( drawingsPath ) )
        , configurationPath_( tools::Path::FromUTF8( configurationPath ) )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventMarkerViewState();
    }
    //@}

    //! @name Member data
    //@{
    std::string label_;
    std::string description_;
    bool resetDrawings_;
    tools::Path drawingsPath_;
    tools::Path configurationPath_;
    //@}
};

} //! namespace gui

#endif // __EventMarkerViewState_h_
