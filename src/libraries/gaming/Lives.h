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
#include "ASN_Types.h"
#include "clients_kernel/Aggregatable_ABC.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  Lives
    @brief  Lives
*/
// Created: AGE 2006-04-10
// =============================================================================
class Lives : public kernel::Extension_ABC
           , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
           , public kernel::Drawable_ABC
           , public kernel::Aggregatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Lives( const kernel::Agent_ABC& agent );
    virtual ~Lives();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
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
    const kernel::Agent_ABC& agent_;
    float life_;
    bool bEmbraye_;
    bool aggregated_;
    //@}
};

#endif // __Lives_h_
