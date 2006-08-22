// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotations_h_
#define __Dotations_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class DotationType;
    class DataDictionary;
}

class Dotation;

// =============================================================================
/** @class  Dotations
    @brief  Dotations. Resources actually
*/
// Created: AGE 2006-02-13
// =============================================================================
class Dotations : public kernel::Extension_ABC
                , public kernel::Updatable_ABC< ASN1T_MsgUnitDotations >
                , public kernel::Resolver< Dotation >
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotations( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType >& resolver, kernel::DataDictionary& dictionary );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dotations( const Dotations& );            //!< Copy constructor
    Dotations& operator=( const Dotations& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::DataDictionary& dictionary_;
    const kernel::Resolver_ABC< kernel::DotationType >& resolver_;
    bool bEmptyGasTank_;
    //@}
};

#endif // __Dotations_h_
