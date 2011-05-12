// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LodgingAttribute_h_
#define __LodgingAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
}

// =============================================================================
/** @class  LodgingAttribute
@brief  LodgingAttribute
*/
// Created: MMC 2011-05-04
// =============================================================================
class LodgingAttribute : public kernel::LodgingAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    LodgingAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~LodgingAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LodgingAttribute( const LodgingAttribute& );            //!< Copy constructor
    LodgingAttribute& operator=( const LodgingAttribute& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_NbcIds;
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
    const tools::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    unsigned int capacity_;
    //@}
};

#endif // __LodgingAttribute_h_
