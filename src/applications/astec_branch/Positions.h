// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Positions_h_
#define __Positions_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class CoordinateConverter;

// =============================================================================
/** @class  Positions
    @brief  Positions. Nothing sexual.
*/
// Created: AGE 2006-03-16
// =============================================================================
class Positions : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Positions( const CoordinateConverter& converter );
    virtual ~Positions();
    //@}

    //! @name Operations
    //@{
    geometry::Point2f GetPosition() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Positions( const Positions& );            //!< Copy constructor
    Positions& operator=( const Positions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter& converter_;
    geometry::Point2f position_;
    //@}
};

#endif // __Positions_h_
