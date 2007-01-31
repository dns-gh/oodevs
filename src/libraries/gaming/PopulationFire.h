// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFire_h_
#define __PopulationFire_h_

#include "game_asn/Asn.h"
#include "Fire_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Population_ABC;
}

// =============================================================================
/** @class  PopulationFire
    @brief  PopulationFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class PopulationFire : public Fire_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationFire( const ASN1T_MsgStartPopulationFire& message, const kernel::Resolver_ABC< kernel::Population_ABC >& resolver );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    //@}
};

#endif // __PopulationFire_h_
