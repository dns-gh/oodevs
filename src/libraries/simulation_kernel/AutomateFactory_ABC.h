// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AutomateFactory_ABC_h_
#define __AutomateFactory_ABC_h_

class MIL_Automate;
class MIL_AutomateType;
class MIL_Formation;

namespace xml
{
    class xistream;
}
// =============================================================================
/** @class  AutomateFactory_ABC
    @brief  AutomateFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class AutomateFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomateFactory_ABC();
    virtual ~AutomateFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Automate* Create( const MIL_AutomateType& type, MIL_Automate& parent, xml::xistream& xis ) = 0;
    virtual MIL_Automate* Create( const MIL_AutomateType& type, MIL_Formation& parent, xml::xistream& xis ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomateFactory_ABC( const AutomateFactory_ABC& );            //!< Copy constructor
    AutomateFactory_ABC& operator=( const AutomateFactory_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AutomateFactory_ABC_h_
