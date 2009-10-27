// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PopulationFactory_h_
#define __PopulationFactory_h_

#include "PopulationFactory_ABC.h"

class DEC_DataBase;

// =============================================================================
/** @class  PopulationFactory
    @brief  PopulationFactory
*/
// Created: MGD 2009-10-24
// =============================================================================
class PopulationFactory : public PopulationFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    PopulationFactory( DEC_DataBase& database );
    virtual ~PopulationFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army& army );
    //@}

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PopulationFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PopulationFactory* factory, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    DEC_DataBase& database_;
    //@}
};

template< typename Archive >
void save_construct_data( Archive& archive, const PopulationFactory* factory, const unsigned int /*version*/ )
{
    const DEC_DataBase* const database = &factory->database_;
    archive << database;
}
template< typename Archive >
void load_construct_data( Archive& archive, PopulationFactory* factory, const unsigned int /*version*/ )
{
    DEC_DataBase* database;
    archive >> database;
    ::new( factory )PopulationFactory( *database );
}

#endif // __PopulationFactory_h_
