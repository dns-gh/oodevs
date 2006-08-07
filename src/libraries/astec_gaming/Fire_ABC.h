// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fire_ABC_h_
#define __Fire_ABC_h_

#include "ASN_Types.h"
#include "astec_kernel/Updatable_ABC.h"
#include "astec_kernel/Drawable_ABC.h"

class Entity_ABC;

// =============================================================================
/** @class  Fire_ABC
    @brief  Fire_ABC
*/
// Created: AGE 2006-03-10
// =============================================================================
class Fire_ABC : public Updatable_ABC< ASN1T_MsgStopPionFire >
               , public Updatable_ABC< ASN1T_MsgStopPopulationFire >
               , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Fire_ABC( Entity_ABC& origin );
    virtual ~Fire_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgStopPionFire& message );
    virtual void DoUpdate( const ASN1T_MsgStopPopulationFire& message );
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const = 0;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Fire_ABC( const Fire_ABC& );
    Fire_ABC& operator=( const Fire_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    Entity_ABC& origin_;
    //@}
};

#endif // __Fire_ABC_h_
