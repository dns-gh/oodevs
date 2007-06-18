// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_NatureAtlas_h_
#define __MissionParameter_NatureAtlas_h_

#include "MissionParameter_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_NatureAtlas
    @brief  MissionParameter_NatureAtlas
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_NatureAtlas : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_NatureAtlas( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_NatureAtlas();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_NatureAtlas( const MissionParameter_NatureAtlas& );            //!< Copy constructor
    MissionParameter_NatureAtlas& operator=( const MissionParameter_NatureAtlas& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_AtlasNature natureAtlas_;
    //@}
};

}

#endif // __MissionParameter_NatureAtlas_h_
