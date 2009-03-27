 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "game_asn/Simulation.h"
#include "ObjectAttribute_ABC.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObstacleAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ObstacleAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg );
    ~ObstacleAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update   ( const ASN1T_ObjectAttributes& asnMsg );
    virtual void Send     ( ASN1T_ObjectAttributes& asnMsg ) const;
    virtual void AsnDelete( ASN1T_ObjectAttributes& asnMsg ) const;
    //@}

private:
    ASN1T_EnumDemolitionTargetType obstacle_;
};

}

#endif // __ObstacleAttribute_h_
