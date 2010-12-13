// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class FireClass;
}

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class FireAttribute : public kernel::FireAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FireAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::FireClass >& resolver );
    virtual ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireAttribute( const FireAttribute& );            //!< Copy constructor
    FireAttribute& operator=( const FireAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::FireClass >& resolver_;
    const kernel::FireClass* fireClass_;
    int heat_;
    //@}
};

#endif // __FireAttribute_h_
