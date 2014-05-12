// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"

namespace sword
{
    class PhaseLineCreation;
}

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Lima
    @brief  Represents a lima.
*/
// Created: APE 2004-03-12
// =============================================================================
class Lima : public TacticalLine_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
             Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const sword::PhaseLineCreation& message );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    virtual void MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual bool IsLimit() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lima( const Lima& );
    Lima& operator=( const Lima& );
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateToSim( E_State state );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __Lima_h_
