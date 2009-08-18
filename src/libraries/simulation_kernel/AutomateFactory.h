// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AutomateFactory_h_
#define __AutomateFactory_h_

#include "AutomateFactory_ABC.h"
#include "clients_kernel/Resolver.h"

class MIL_IDManager;

// =============================================================================
/** @class  AutomateFactory
    @brief  AutomateFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AutomateFactory : public AutomateFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomateFactory( MIL_IDManager& idManager );
    virtual ~AutomateFactory();
    //@}

    //! @name Operations
    //@{
    //! @name Operations
    //@{
    virtual MIL_Automate* Create( const MIL_AutomateType& type, MIL_Automate& parent, xml::xistream& xis );
    virtual MIL_Automate* Create( const MIL_AutomateType& type, MIL_Formation& parent, xml::xistream& xis );
    //@}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomateFactory( const AutomateFactory& );            //!< Copy constructor
    AutomateFactory& operator=( const AutomateFactory& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    //@}
};

#endif // __AutomateFactory_h_
