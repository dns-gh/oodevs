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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"
#include "Drawable_ABC.h"

class Controller;
class DotationType;
class Dotation;
class DataDictionary;

// =============================================================================
/** @class  Dotations
    @brief  Dotations. Resources actually
*/
// Created: AGE 2006-02-13
// =============================================================================
class Dotations : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgUnitDotations >
                , public Resolver< Dotation >
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotations( Controller& controller, const Resolver_ABC< DotationType >& resolver, DataDictionary& dictionary );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
    Controller& controller_;
    DataDictionary& dictionary_;
    const Resolver_ABC< DotationType >& resolver_;
    bool bEmptyGasTank_;
    //@}
};

#endif // __Dotations_h_
