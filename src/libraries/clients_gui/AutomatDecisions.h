// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatDecisions_h_
#define __AutomatDecisions_h_

#include "Decisions.h"

#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class Displayer_ABC;
    class DecisionalModel;
    class AutomatType;
}

namespace sword
{
    class AutomatAttributes;
    class AutomatOrder;
}

namespace tools
{
    template< typename T, typename U > class Resolver_ABC;
}

namespace xml
{
    class xistream;
}

class Publisher_ABC;

namespace gui
{

// =============================================================================
/** @class  AutomatDecisions
    @brief  Automat decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public Decisions
                       , public kernel::Displayable_ABC
                       , public kernel::Serializable_ABC
                       , public kernel::Updatable_ABC< sword::AutomatAttributes >
                       , public kernel::Updatable_ABC< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( kernel::Controller& controller,
                               const kernel::Automat_ABC& agent,
                               const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver );
             AutomatDecisions( xml::xistream& xis,
                               kernel::Controller& controller,
                               const kernel::Automat_ABC& agent,
                               const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver );
    virtual ~AutomatDecisions();
    //@}

    //! @name Decisions_ABC implementation
    //@{
    virtual bool CanBeOrdered() const;
    //@}

    //! @name Operations
    //@{
    bool IsEngaged() const;
    void SetEngaged( bool engaged );
    //@}

private:
    //! @name kernel::Displayable_ABC implementation
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name kernel::Serializable_ABC implementation
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name kernel::UpdateTable_ABC implementation
    //@{
    virtual void DoUpdate( const sword::AutomatAttributes& message );
    virtual void DoUpdate( const sword::AutomatOrder& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver_;
    bool engaged_;
    //@}
};

} //! namespace gui

#endif // __AutomatDecisions_h_
