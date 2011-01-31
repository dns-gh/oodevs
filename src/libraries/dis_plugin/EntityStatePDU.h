// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EntityStatePDU_h_
#define __EntityStatePDU_h_

#include "DisHeader.h"
#include "EntityMarking.h"
#include "rpr_tools/EntityIdentifier.h"
#include "rpr_tools/EntityType.h"
#include "rpr_tools/Coordinates.h"

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  EntityStatePDU
    @brief  EntityStatePDU
*/
// Created: AGE 2008-03-10
// =============================================================================
class EntityStatePDU
{
public:
    //! @name Constants
    //@{
    enum { size = 144 };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             EntityStatePDU( unsigned long time, unsigned char exercise, const rpr::EntityIdentifier& id );
    virtual ~EntityStatePDU();
    //@}

    //! @name Modifiers
    //@{
    void SetForceId( unsigned char forceId );
    void SetEntityName( const std::string& name );
    void SetEntityType( const rpr::EntityType& type );
    void SetPosition( double latitude, double longitude, float altitude, float speed, float heading );

    void SetAppearance( unsigned damageLevel, bool smoking, bool flaming );
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        header_.Serialize( archive );
        id_    .Serialize( archive );
        archive << forceID_ << numberOfArticulationParameters_;
        entityType_.Serialize( archive );
        alternativeType_.Serialize( archive );

        entityLinearVelocity_.Serialize( archive );
        entityLocation_.Serialize( archive );
        entityOrientation_.Serialize( archive );

        archive << appearance_ << deadReckoningAlgorithm_ << otherParameters_;
        DRentityLinearAcceleration_.Serialize( archive );
        DRentityAngularVelocity_.Serialize( archive );
        entityMarking_.Serialize( archive );
        archive << capabilities_;
    }
    //@}

private:
    //! @name Member data
    //@{
    DisHeader             header_;                          // 12
    rpr::EntityIdentifier id_;                              // 6
    unsigned char         forceID_;                         // 1
    unsigned char         numberOfArticulationParameters_;  // 1    20
    rpr::EntityType       entityType_;                      // 8
    rpr::EntityType       alternativeType_;                 // 8    36
    rpr::VelocityVector   entityLinearVelocity_;            // 12   48
    rpr::WorldLocation    entityLocation_;                  // 24   72
    rpr::Orientation      entityOrientation_;               // 12   84
    unsigned long         appearance_;                      // 4    88
    unsigned char         deadReckoningAlgorithm_;          // 1
    unsigned char         otherParameters_[15];             // 15   104
    rpr::VelocityVector   DRentityLinearAcceleration_;      // 12   116
    rpr::VelocityVector   DRentityAngularVelocity_;         // 12   128
    EntityMarking         entityMarking_;                   // 12   140
    unsigned long         capabilities_;                    // 4    144
    //@}
};

}
}

#endif // __EntityStatePDU_h_
