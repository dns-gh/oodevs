// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Limits_h_
#define __Limits_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

class TacticalLine_ABC;

// =============================================================================
/** @class  Limits
    @brief  Limits
*/
// Created: AGE 2006-02-13
// =============================================================================
class Limits : public kernel::Extension_ABC
             , public kernel::Updatable_ABC< ASN1T_MsgPionOrder >
             , public kernel::Updatable_ABC< ASN1T_MsgPionOrderAck >
{

public:
    //! @name Constructors/Destructor
    //@{
             Limits( const kernel::Resolver_ABC< TacticalLine_ABC >& resolver );
    virtual ~Limits();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Limits( const Limits& );            //!< Copy constructor
    Limits& operator=( const Limits& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPionOrder& message );
    virtual void DoUpdate( const ASN1T_MsgPionOrderAck& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< TacticalLine_ABC >& resolver_;

    bool acknowledged_;
    TacticalLine_ABC* left_;
    TacticalLine_ABC* right_;
    //@}
};

#endif // __Limits_h_
