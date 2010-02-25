// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_LocationList_h_
#define __MissionParameter_LocationList_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_LocationList
    @brief  MissionParameter_LocationList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_LocationList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_LocationList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_LocationList();
    //@}

    //! @name Operations
    //@{
    virtual void Send( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_LocationList( const MissionParameter_LocationList& );            //!< Copy constructor
    MissionParameter_LocationList& operator=( const MissionParameter_LocationList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Localisation > T_LocalisationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_LocalisationVector locations_;
    //@}
};

}

#endif // __MissionParameter_LocationList_h_
