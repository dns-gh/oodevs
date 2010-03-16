// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoModel_h_
#define __MeteoModel_h_

#include "MeteoManager_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter;
}
class Publisher_ABC;

// =============================================================================
/** @class  MeteoModel
    @brief  MeteoModel
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoModel : public MeteoManager_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MeteoModel( kernel::CoordinateConverter& conv, Publisher_ABC& pub );
    virtual ~MeteoModel();
    //@}

    //! @name Operations
    //@{

    virtual void                 RegisterMeteo ( PHY_Meteo& );
    virtual void                 UnregisterMeteo( PHY_Meteo& );
    virtual const PHY_Lighting&  GetLighting  () const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoModel( const MeteoModel& );            //!< Copy constructor
    MeteoModel& operator=( const MeteoModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::set< PHY_Meteo* >     T_MeteoSet;
    typedef T_MeteoSet::const_iterator CIT_MeteoSet;
    virtual void OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg );    
    virtual void OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg ); 
   //@}

private:
    //! @name Member data
    //@{
    PHY_Meteo*                       pGlobalMeteo_;
    T_MeteoSet                       meteos_;    // Including global meteo
    Publisher_ABC&                   publisher_;
    kernel::CoordinateConverter&         converter_;
    //@}
};

#endif // __MeteoModel_h_
