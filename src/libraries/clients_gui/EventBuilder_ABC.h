// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventBuilder_ABC_h_
#define __EventBuilder_ABC_h_

#include "ENT/ENT_Tr.h"
#include <boost/noncopyable.hpp>
#include <vector>

namespace gui
{
// =============================================================================
/** @class  EventBuilder_ABC
    @brief  Event builder declaration
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventBuilder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventBuilder_ABC() {}
    virtual ~EventBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( const std::vector< E_MissionType >& types, E_MissionType currentType,
                        const std::vector< std::string >& missions, const std::string& currentMission,
                        const std::vector< std::string >& disabledMissions, bool invalid, bool missionSelector ) = 0;
    //@}
};

}

#endif // __EventBuilder_ABC_h_
