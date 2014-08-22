// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class ParentEntity;
}

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
    class Displayer_ABC;
}

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticAttribute : public kernel::LogisticAttribute_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    kernel::Entity_ABC* logisticSuperior_;
    //@}
};

#endif // __LogisticAttribute_h_
