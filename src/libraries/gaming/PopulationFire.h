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

#include "Fire_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

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
             PopulationFire( const MsgsSimToClient::MsgStartPopulationFire& message, const tools::Resolver_ABC< kernel::Population_ABC >& resolver );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    //@}
};

#endif // __PopulationFire_h_
