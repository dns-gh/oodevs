// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_IsPartOfStruct_h
#define plugins_hla_IsPartOfStruct_h

#include "Omt13String.h"
#include "rpr/Coordinates.h"
#include "rpr/EntityIdentifier.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  IsPartOfStruct
    @brief  IsPartOfStruct
*/
// Created: AHC 2012-10-02
// =============================================================================
class IsPartOfStruct
{
public:
    //! @name Constructors/Destructor
    //@{
             IsPartOfStruct();
             IsPartOfStruct( const rpr::EntityIdentifier& disId, const Omt13String& rtiId );
    virtual ~IsPartOfStruct();
    //@}
    
    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {    
        unsigned short constituentPartNature = 0;
        unsigned short constituentPartPosition = 0;
        short stationNumber = 0;
        unsigned short constituentPartSationName = 0;
        float x=0.f, y=0.f;

        archive << disId_
                << rtiId_
                << constituentPartNature
                << constituentPartPosition
                << stationNumber
                << constituentPartSationName
                << x
                << y;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        unsigned short constituentPartNature = 0;
        unsigned short constituentPartPosition = 0;
        short stationNumber = 0;
        unsigned short constituentPartSationName = 0;
        float x, y;
        archive >> disId_
                >> rtiId_
                >> constituentPartNature
                >> constituentPartPosition
                >> stationNumber
                >> constituentPartSationName
                >> x
                >> y;
    }
    //@}

public:
    //! @name Member data
    //@{
    rpr::EntityIdentifier disId_;
    Omt13String rtiId_;
    //@}
};

}
}

#endif // plugins_hla_IsPartOfStruct_h
