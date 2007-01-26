// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Master_h_
#define __Master_h_

#include "network/Master_Asn.h"
#include "network/Server_ABC.h"

namespace DIN
{
    class DIN_Input;
}

namespace frontend
{

// =============================================================================
/** @class  Master
    @brief  Master
*/
// Created: SBO 2007-01-25
// =============================================================================
class Master : public network::Server_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Master( DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
    virtual ~Master();
    //@}

    //! @name Operations
    //@{
    void OnReceive( const ASN1T_MsgsOutMaster& message );
    void OnReceive( unsigned int id, DIN::DIN_Input& message );
    void Send     ( const ASN1T_MsgsInMaster&  message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Master( const Master& );            //!< Copy constructor
    Master& operator=( const Master& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __Master_h_
