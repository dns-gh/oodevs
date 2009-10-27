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

class DEC_DataBase;
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
    explicit AutomateFactory( MIL_IDManager& idManager, DEC_DataBase& database  );
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
    template< typename Archive > friend  void save_construct_data( Archive& archive, const AutomateFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, AutomateFactory* factory, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    DEC_DataBase& database_;
    //@}
};

template< typename Archive >
void save_construct_data( Archive& archive, const AutomateFactory* factory, const unsigned int /*version*/ )
{
    const MIL_IDManager* const idManager = &factory->idManager_;
    const DEC_DataBase* const database = &factory->database_;
    archive << idManager
        << database;
}
template< typename Archive >
void load_construct_data( Archive& archive, AutomateFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    DEC_DataBase* database;
    archive >> idManager
        >> database;
    ::new( factory )AutomateFactory( *idManager, *database );
}

#endif // __AutomateFactory_h_
