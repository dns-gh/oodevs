// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_ABC_h_
#define __Client_ABC_h_

#include "DIN/DIN_UserData_ABC.h"

namespace DIN
{
    class DIN_BufferedMessage;
    class DIN_Link;
}

namespace tools
{
    class ObjectMessageService;

// =============================================================================
/** @class  Client_ABC
    @brief  Client_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client_ABC : public DIN::DIN_UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client_ABC( ObjectMessageService& messageService, DIN::DIN_Link& link );
    virtual ~Client_ABC();
    //@}

    //! @name Messages
    //@{
    DIN::DIN_BufferedMessage BuildDinMsg();
    //@}

    //! @name Tools
    //@{
    void Disconnect();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Client_ABC( const Client_ABC& );            //!< Copy constructor
    Client_ABC& operator=( const Client_ABC& ); //!< Assignement operator
    //@}

protected: // $$$$ AGE 2007-08-23: 
    ObjectMessageService& messageService_;
    DIN::DIN_Link&        link_;
};

}

#endif // __Client_ABC_h_
