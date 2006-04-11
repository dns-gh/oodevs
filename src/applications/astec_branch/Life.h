// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Life_h_
#define __Life_h_

#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"
#include "ASN_Types.h"

// =============================================================================
/** @class  Life
    @brief  Life
*/
// Created: AGE 2006-04-10
// =============================================================================
class Life : public Extension_ABC
           , public Updatable_ABC< ASN1T_MsgUnitAttributes >
           , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Life( bool automat );
    virtual ~Life();
    //@}

    //! @name Operations
    //@{
    // $$$$ AGE 2006-04-11: 
    void DrawAggregated( const geometry::Point2f& where, const GlTools_ABC& tools ) const;

    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Life( const Life& );            //!< Copy constructor
    Life& operator=( const Life& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    float life_;
    bool automat_;
    bool bEmbraye_;
    //@}
};

#endif // __Life_h_
