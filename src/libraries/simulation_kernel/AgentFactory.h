// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"
#include "tools/Resolver.h"

class DEC_DataBase;
class MIL_IDManager;
class AlgorithmsFactories;

// =============================================================================
/** @class  AgentFactory
    @brief  AgentFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( MIL_IDManager& idManager, DEC_DataBase& database );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void Initialize( MIL_AgentPion& pion, MIL_Automate& automate, const MT_Vector2D& vPosition );
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const AgentFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, AgentFactory* factory, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    std::auto_ptr< AlgorithmsFactories > algorithmsFactories_;
    DEC_DataBase& database_;
    //@}
};


template< typename Archive >
void save_construct_data( Archive& archive, const AgentFactory* factory, const unsigned int /*version*/ )
{
    const MIL_IDManager* const idManager = &factory->idManager_;
    const DEC_DataBase* const database = &factory->database_;
    archive << idManager
        << database;
}
template< typename Archive >
void load_construct_data( Archive& archive, AgentFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    DEC_DataBase* database;
    archive >> idManager
        >> database;
    ::new( factory )AgentFactory( *idManager, *database );
}

#endif // __AgentFactory_h_
