// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_EquipmentType_h_
#define __MissionParameter_EquipmentType_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_EquipmentType
    @brief  MissionParameter_EquipmentType
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_EquipmentType : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_EquipmentType( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_EquipmentType();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_EquipmentType( const MissionParameter_EquipmentType& );            //!< Copy constructor
    MissionParameter_EquipmentType& operator=( const MissionParameter_EquipmentType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int equipmentType_;
    //@}
};

}

#endif // __MissionParameter_EquipmentType_h_
