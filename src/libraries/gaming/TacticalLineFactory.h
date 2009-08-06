// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineFactory_h_
#define __TacticalLineFactory_h_

#include "game_asn/Simulation.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

class TacticalLine_ABC;
class Publisher_ABC;
class Model;

// =============================================================================
/** @class  TacticalLineFactory
    @brief  TacticalLineFactory
*/
// Created: SBO 2006-11-17
// =============================================================================
class TacticalLineFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineFactory( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, Model& model, Publisher_ABC& publisher );
    virtual ~TacticalLineFactory();
    //@}

    //! @name Operations
    //@{
    TacticalLine_ABC* Create( const ASN1T_MsgLimaCreation&  asnMsg );
    TacticalLine_ABC* Create( const ASN1T_MsgLimitCreation& asnMsg );

    void CreateLimit( const T_PointVector& points, const kernel::Entity_ABC& superior );
    void CreateLima ( const T_PointVector& points, const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLineFactory( const TacticalLineFactory& );            //!< Copy constructor
    TacticalLineFactory& operator=( const TacticalLineFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    Model& model_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __TacticalLineFactory_h_
