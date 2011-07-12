// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_DatabaseUpdater_ABC_h_
#define __crossbow_DatabaseUpdater_ABC_h_

namespace sword
{
    class FormationCreation;
    class AutomatCreation;
    class AutomatAttributes;
    class UnitCreation;
    class UnitAttributes;
    class UnitDestruction;
    class UnitKnowledgeCreation;
    class UnitKnowledgeUpdate;
    class UnitKnowledgeDestruction;
    class ObjectCreation;
    class ObjectDestruction;
    class ObjectUpdate;
    class ObjectKnowledgeCreation;
    class ObjectKnowledgeUpdate;
    class ObjectKnowledgeDestruction;
    class Report;
    class PartyCreation;
    class ControlBeginTick;
    class ObjectMagicActionAck;
    class LimitCreation;
    class PhaseLineCreation;
    class UrbanCreation;
    class UrbanUpdate;
    class PopulationCreation;
    class PopulationUpdate;
}

namespace plugins
{
namespace crossbow
{
// =============================================================================
/** @class  DatabaseUpdater
    @brief  DatabaseUpdater
*/
// Created: JCR 2007-04-30
// =============================================================================
class DatabaseUpdater_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            DatabaseUpdater_ABC() {}
    virtual ~DatabaseUpdater_ABC() {}
    //@}

    //! @name sword
    //@{
    virtual void Update( const sword::LimitCreation& msg ) = 0;
    virtual void Update( const sword::PhaseLineCreation& msg ) = 0;
    //@}

    //! @name sword
    //@{
    virtual void Update( const sword::UnitCreation& msg ) = 0;
    virtual void Update( const sword::UnitAttributes& msg ) = 0;
    virtual void Update( const sword::UnitDestruction& msg ) = 0;

    virtual void Update( const sword::AutomatCreation& message ) = 0;
    virtual void Update( const sword::AutomatAttributes& msg ) = 0;

    virtual void Update( const sword::UnitKnowledgeCreation& msg ) = 0;
    virtual void Update( const sword::UnitKnowledgeUpdate& msg ) = 0;
    virtual void Update( const sword::UnitKnowledgeDestruction& msg ) = 0;

    virtual void Update( const sword::ObjectCreation& msg ) = 0;
    virtual void Update( const sword::ObjectUpdate& msg ) = 0;
    virtual void Update( const sword::ObjectDestruction& msg ) = 0;

    virtual void Update( const sword::ObjectKnowledgeCreation& msg ) = 0;
    virtual void Update( const sword::ObjectKnowledgeUpdate& msg ) = 0;
    virtual void Update( const sword::ObjectKnowledgeDestruction& msg ) = 0;

    virtual void Update( const sword::UrbanCreation& msg ) = 0;
    virtual void Update( const sword::UrbanUpdate& msg ) = 0;

    virtual void Update( const sword::FormationCreation& message ) = 0;

    virtual void Update( const sword::ControlBeginTick& msg ) = 0;
    virtual void Update( const sword::PartyCreation& msg ) = 0;
    virtual void Update( const sword::PopulationCreation& msg ) = 0;
    virtual void Update( const sword::PopulationUpdate& msg ) = 0;
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
