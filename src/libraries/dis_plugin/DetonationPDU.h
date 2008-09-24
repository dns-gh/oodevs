// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DetonationPDU_h_
#define __DetonationPDU_h_

#include "DisHeader.h"
#include "BurstDescriptor.h"
#include "hla_plugin/EntityIdentifier.h"
#include "hla_plugin/Coordinates.h"

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  DetonationPDU
    @brief  DetonationPDU
*/
// Created: AGE 2008-04-08
// =============================================================================
class DetonationPDU
{
public:
    //! @name Types
    //@{
    enum { size = 104 };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DetonationPDU( const hla::EntityIdentifier& firer, unsigned long time, unsigned char exercise );
    virtual ~DetonationPDU();
    //@}

    //! @name Operations
    //@{
    void SetBurst   ( unsigned short quantity, unsigned seconds, BurstDescriptor::warhead w );
    void SetPosition( double latitude, double longitude, float altitude );
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        header_  .Serialize( archive );
        firer_   .Serialize( archive );
        target_  .Serialize( archive );
        event_   .Serialize( archive );
        velocity_.Serialize( archive );
        location_.Serialize( archive );
        burst_   .Serialize( archive );

        archive << locInEntityCoords_
                << detonationResult_ << numberOfArticulationParameters_ << padding_;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DetonationPDU( const DetonationPDU& );            //!< Copy constructor
    DetonationPDU& operator=( const DetonationPDU& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DisHeader             header_;                               // 12
    hla::EntityIdentifier firer_;                                // 6    18
    hla::EntityIdentifier target_;                               // 6    24
    hla::EntityIdentifier munition_;                             // 6    30
    hla::EntityIdentifier event_;                                // 6    36
    hla::VelocityVector   velocity_;                             // 12   48
    hla::WorldLocation    location_;                             // 24   72
    BurstDescriptor       burst_;                                // 16   88
    float                 locInEntityCoords_[3];                 // 12   100
    unsigned char         detonationResult_;                     // 1
    unsigned char         numberOfArticulationParameters_;       // 1
    unsigned short        padding_;                              // 2    104
    //@}
};

}
}

#endif // __DetonationPDU_h_
