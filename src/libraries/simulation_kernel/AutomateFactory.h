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
    explicit AutomateFactory( MIL_IDManager& idManager );
    virtual ~AutomateFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Automate& parent );
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Formation& parent );
    //@}
private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const AutomateFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, AutomateFactory* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    //@}
};

#endif // __AutomateFactory_h_
