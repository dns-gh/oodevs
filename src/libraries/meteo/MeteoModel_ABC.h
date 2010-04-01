// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoModel_ABC_h_
#define __MeteoModel_ABC_h_

#include "MeteoManager_ABC.h"

// =============================================================================
/** @class  MeteoModel_ABC
    @brief  MeteoModel_ABC
*/
// Created: HBD 2010-03-23
// =============================================================================
class MeteoModel_ABC : public MeteoManager_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MeteoModel_ABC() {};
    virtual ~MeteoModel_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Lighting& GetLighting() const = 0;
    virtual void OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& message ) = 0;
    virtual void OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& message ) = 0;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterMeteo  ( PHY_Meteo& ) = 0;
    virtual void UnregisterMeteo( PHY_Meteo& ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoModel_ABC( const MeteoModel_ABC& );            //!< Copy constructor
    MeteoModel_ABC& operator=( const MeteoModel_ABC& ); //!< Assignment operator
    //@}
};

#endif // __MeteoModel_ABC_h_
