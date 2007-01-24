// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lives_h_
#define __Lives_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "network/Simulation_Asn.h"

// =============================================================================
/** @class  Lives
    @brief  Lives
*/
// Created: AGE 2006-04-10
// =============================================================================
class Lives : public kernel::Extension_ABC
            , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
            , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Lives();
    virtual ~Lives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );

    float GetLife() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lives( const Lives& );            //!< Copy constructor
    Lives& operator=( const Lives& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    float life_;
    //@}
};

#endif // __Lives_h_
