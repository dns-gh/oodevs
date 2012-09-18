// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"

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
             Lima( kernel::Controller& controller, tools::IdManager& idManager );
             Lima( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& actions ) const;
    virtual void MultipleSelect( kernel::ActionController& controller, const std::vector< const kernel::Selectable_ABC* >& elements ) const;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& point ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual bool IsLimit() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lima( const Lima& );
    Lima& operator=( const Lima& );
    //@}

private:
    kernel::Controller& controller_;
    //@}
};

#endif // __Lima_h_
