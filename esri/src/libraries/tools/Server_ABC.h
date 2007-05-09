// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Server_ABC_h_
#define __Server_ABC_h_

#include "DIN/DIN_UserData_ABC.h"

namespace DIN
{
    class DIN_MessageService_ABC;
    class DIN_BufferedMessage;
    class DIN_Link;
}

namespace tools
{
// =============================================================================
/** @class  Server_ABC
    @brief  Server_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class Server_ABC : public DIN::DIN_UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Server_ABC( DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link );
    virtual ~Server_ABC();
    //@}

    //! @name Messages
    //@{
    DIN::DIN_BufferedMessage BuildDinMsg();
    //@}

    //! @name Tools
    //@{
    static Server_ABC& GetServerFromLink( const DIN::DIN_Link& link );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Server_ABC( const Server_ABC& );            //!< Copy constructor
    Server_ABC& operator=( const Server_ABC& ); //!< Assignement operator
    //@}

protected:
    DIN::DIN_MessageService_ABC& messageService_;
    DIN::DIN_Link&               link_;
};

}

#endif // __Server_ABC_h_
