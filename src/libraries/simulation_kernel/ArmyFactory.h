// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ArmyFactory_h_
#define __ArmyFactory_h_

#include "ArmyFactory_ABC.h"
#include "Entities/MIL_Army.h"

class AutomateFactory_ABC;
class FormationFactory_ABC;
class MIL_ObjectManager;
class PopulationFactory_ABC;
class KnowledgeGroupFactory_ABC;

// =============================================================================
/** @class  ArmyFactory
    @brief  ArmyFactory
*/
// Created: MGD 2009-10-24
// =============================================================================
class ArmyFactory : public ArmyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );//@TODO MGD rename and adapt MIL_ObjectManager
    virtual ~ArmyFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Army_ABC* Create( xml::xistream& xis );
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeDiplomacies();
    //@}

private:
    //! @name Operations
    //@{
    AutomateFactory_ABC& automateFactory_;
    FormationFactory_ABC& formationFactory_;
    MIL_ObjectManager& objectFactory_;
    PopulationFactory_ABC& populationFactory_;
    KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
    std::auto_ptr< MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy > > diplomacyConverter_;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const ArmyFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, ArmyFactory* role, const unsigned int /*version*/ );
    //@}
};

#endif // __ArmyFactory_h_
