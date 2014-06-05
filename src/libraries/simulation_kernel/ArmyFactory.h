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
#include "MT_Tools/MT_Converter.h"
#include "MT_Tools/MT_String.h"
#include "ENT/ENT_Enums.h"

class AutomateFactory_ABC;
class FormationFactory_ABC;
class MIL_ObjectManager;
class PopulationFactory_ABC;
class InhabitantFactory_ABC;
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
    explicit ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory,
                          MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory,
                          InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    virtual ~ArmyFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Army_ABC* Create( const std::string& tag, xml::xistream& xis, const MIL_Config& config );
    virtual void Finalize();
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const ArmyFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, ArmyFactory* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNoSideObject( xml::xistream& xis );
    void InitializeDiplomacies();
    //@}

private:
    //! @name Data Members
    //@{
    AutomateFactory_ABC& automateFactory_;
    FormationFactory_ABC& formationFactory_;
    MIL_ObjectManager& objectFactory_;
    PopulationFactory_ABC& populationFactory_;
    InhabitantFactory_ABC& inhabitantFactory_;
    KnowledgeGroupFactory_ABC& knowledgeGroupFactory_; // LTO
    std::unique_ptr< MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess > > diplomacyConverter_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ArmyFactory )

#endif // __ArmyFactory_h_
