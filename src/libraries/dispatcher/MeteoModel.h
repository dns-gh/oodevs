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

#include "meteo/MeteoModel_ABC.h"
#include "protocol/Protocol.h"
#include <list>

    
namespace kernel
{
    class CoordinateConverter_ABC;
    class CoordinateConverter;
}
class PHY_Meteo;

namespace dispatcher 
{

// =============================================================================
/** @class  MeteoModel
    @brief  MeteoModel
*/
// Created: HBD 2010-03-23
// =============================================================================
class MeteoModel : public MeteoModel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MeteoModel( kernel::CoordinateConverter_ABC& conv );
    virtual ~MeteoModel();
    //@}


    virtual const PHY_Lighting&  GetLighting  () const;
    virtual void OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg );    
    virtual void OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg ); 
    //@}

protected:

    virtual void     RegisterMeteo ( PHY_Meteo& );
    virtual void     UnregisterMeteo( PHY_Meteo& );

private:

   typedef std::list< PHY_Meteo* >     T_MeteoList;
   typedef T_MeteoList::iterator CIT_MeteoList;

private:
    //! @name Member data
    //@{
    PHY_Meteo*                        pGlobalMeteo_;
    T_MeteoList                        meteos_;    // Including global meteo
    kernel::CoordinateConverter_ABC&  converter_;
    //@}
};
}

#endif // __MeteoModel_h_
