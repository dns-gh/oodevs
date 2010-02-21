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
#include "protocol/protocol.h"

namespace Common
{
    class MsgAtlasNature;
}

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
             MissionParameter_NatureAtlas( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_NatureAtlas();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
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
    Common::MsgAtlasNature natureAtlas_;
    //@}
};

}

#endif // __MissionParameter_NatureAtlas_h_
