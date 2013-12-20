// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_test_pch.h"

#include "MockEventTools.h"
#include "clients_gui/EventMagicViewState.h"

namespace gui
{
    std::ostream& operator<<( std::ostream& os, const gui::EventMagicViewState& state )
    {
        os << std::endl << "EventMagicViewState " << "{ "
           << state.name_ << ", "
           << state.target_ << ", "
           << state.hasTarget_ << " }" << std::endl;
        return os;
    }
}

namespace
{
    bool operator==( const gui::EventMagicViewState& lhs, const gui::EventMagicViewState& rhs )
    {
        return lhs.target_ == rhs.target_ &&
               lhs.name_ == rhs.name_ &&
               lhs.hasTarget_ == rhs.hasTarget_;
    }
}