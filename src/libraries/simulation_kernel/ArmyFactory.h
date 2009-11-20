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

class AutomateFactory_ABC;
class FormationFactory_ABC;
class MIL_ObjectManager;
class PopulationFactory_ABC;

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
    //@TODO MGD rename and adapt MIL_ObjectManager
    explicit ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory );
    virtual ~ArmyFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Army* Create( xml::xistream& xis );
    //@}

private:
    //! @name Operations
    //@{
    AutomateFactory_ABC& automateFactory_;
    FormationFactory_ABC& formationFactory_;
    MIL_ObjectManager& objectFactory_;
    PopulationFactory_ABC& populationFactory_;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const ArmyFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, ArmyFactory* role, const unsigned int /*version*/ );
    //@}
};

#endif // __ArmyFactory_h_
