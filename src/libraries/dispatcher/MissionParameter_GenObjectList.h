// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_GenObjectList_h_
#define __MissionParameter_GenObjectList_h_

#include "MissionParameter_ABC.h"
#include "GenObject.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_GenObjectList
    @brief  MissionParameter_GenObjectList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_GenObjectList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_GenObjectList( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_GenObjectList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( Common::MsgMissionParameter& asn ) const;
    virtual void Delete( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_GenObjectList( const MissionParameter_GenObjectList& );            //!< Copy constructor
    MissionParameter_GenObjectList& operator=( const MissionParameter_GenObjectList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< GenObject > T_GenObjectVector;
    //@}

private:
    //! @name Member data
    //@{
    T_GenObjectVector objects_;
    //@}
};

}

#endif // __MissionParameter_GenObjectList_h_
