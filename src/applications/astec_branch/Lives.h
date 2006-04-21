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

#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"
#include "ASN_Types.h"
#include "Aggregatable_ABC.h"

class Agent;

// =============================================================================
/** @class  Lives
    @brief  Lives
*/
// Created: AGE 2006-04-10
// =============================================================================
class Lives : public Extension_ABC
           , public Updatable_ABC< ASN1T_MsgUnitAttributes >
           , public Drawable_ABC
           , public Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Lives( const Agent& agent );
    virtual ~Lives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lives( const Lives& );            //!< Copy constructor
    Lives& operator=( const Lives& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    float GetLife() const;
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    float life_;
    bool bEmbraye_;
    bool aggregated_;
    //@}
};

#endif // __Lives_h_
