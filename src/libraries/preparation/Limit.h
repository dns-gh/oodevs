// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Limit_h_
#define __Limit_h_

#include "TacticalLine_ABC.h"

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Limit
    @brief  Represents a limit.
*/
// Created: APE 2004-03-12
// =============================================================================
class Limit : public TacticalLine_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Limit( kernel::Controller& controller, tools::IdManager& idManager );
             Limit( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual bool IsLimit() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Limit( const Limit& );
    Limit& operator=( const Limit& );
    //@}

private:
    kernel::Controller& controller_;
    //@}
};

#endif // __Limit_h_
