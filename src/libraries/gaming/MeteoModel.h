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

// =============================================================================
/** @class  MeteoModel
    @brief  MeteoModel
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoModel : public MeteoModel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MeteoModel( kernel::CoordinateConverter_ABC& conv );
    virtual ~MeteoModel();
    //@}

    //! @name Operations
    //@{


    virtual const PHY_Lighting&  GetLighting  () const;
    virtual void OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg );    
    virtual void OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg ); 
    //@}

protected:

    virtual void     RegisterMeteo ( PHY_Meteo& );
    virtual void     UnregisterMeteo( PHY_Meteo& );

private:
    //! @name Helpers
    //@{
    typedef std::list< PHY_Meteo* >     T_MeteoList;
    typedef T_MeteoList::const_iterator CIT_MeteoList;
    typedef T_MeteoList::iterator       IT_MeteoList;
   //@}

private:
    //! @name Member data
    //@{
    PHY_Meteo*                        pGlobalMeteo_;
    T_MeteoList                        meteos_; 
    kernel::CoordinateConverter_ABC&  converter_;
    //@}
};

#endif // __MeteoModel_h_
