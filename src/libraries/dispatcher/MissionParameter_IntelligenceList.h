// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_IntelligenceList_h_
#define __MissionParameter_IntelligenceList_h_

#include "MissionParameter_ABC.h"
#include "IntelligenceOrder.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_IntelligenceList
    @brief  MissionParameter_IntelligenceList
*/
// Created: SBO 2008-03-04
// =============================================================================
class MissionParameter_IntelligenceList : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_IntelligenceList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_IntelligenceList();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_IntelligenceList( const MissionParameter_IntelligenceList& );            //!< Copy constructor
    MissionParameter_IntelligenceList& operator=( const MissionParameter_IntelligenceList& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< IntelligenceOrder > intelligenceOrders_;
    //@}
};

}

#endif // __MissionParameter_IntelligenceList_h_
