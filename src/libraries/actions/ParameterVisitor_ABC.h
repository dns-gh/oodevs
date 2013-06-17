// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterVisitor_ABC_h_
#define __ActionParameterVisitor_ABC_h_

namespace actions
{
    class Parameter_ABC;

    namespace parameters {
        class Agent;
        class AtlasNature;
        class Army;
        class Automat;
        class Bool;
        class Crowd;
        class DateTime;
        class Direction;
        class DotationType;
        class EngineerConstruction;
        class Enumeration;
        class ExtensionList;
        class Identifier;
        class Inhabitant;
        class KnowledgeGroup;
        class Lima;
        class Location;
        class MaintenancePriorities;
        class MedicalPriorities;
        class Numeric;
        class ObjectKnowledge;
        class Path;
        class PathPoint;
        class PopulationKnowledge;
        class Quantity;
        class Resource;
        class ResourceNetworkNode;
        class ResourceNetworkType;
        class String;
        class UrbanBlock;
    }

// =============================================================================
/** @class  ParameterVisitor_ABC
    @brief  ParameterVisitor_ABC
*/
// Created: SBO 2007-05-22
// =============================================================================
class ParameterVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParameterVisitor_ABC() {}
    virtual ~ParameterVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const parameters::Agent&                 ) {}
    virtual void Visit( const parameters::AtlasNature&           ) {}
    virtual void Visit( const parameters::Automat&               ) {}
    virtual void Visit( const parameters::Army&                  ) {}
    virtual void Visit( const parameters::Bool&                  ) {}
    virtual void Visit( const parameters::Crowd&                 ) {}
    virtual void Visit( const parameters::DateTime&              ) {}
    virtual void Visit( const parameters::Direction&             ) {}
    virtual void Visit( const parameters::DotationType&          ) {}
    virtual void Visit( const parameters::EngineerConstruction&  ) {}
    virtual void Visit( const parameters::Enumeration&           ) {}
    virtual void Visit( const parameters::ExtensionList&         ) {}
    virtual void Visit( const parameters::Identifier&            ) {}
    virtual void Visit( const parameters::Inhabitant&            ) {}
    virtual void Visit( const parameters::KnowledgeGroup&        ) {}
    virtual void Visit( const parameters::Lima&                  ) {}
    virtual void Visit( const parameters::Location&              ) {}
    virtual void Visit( const parameters::MaintenancePriorities& ) {}
    virtual void Visit( const parameters::MedicalPriorities&     ) {}
    virtual void Visit( const parameters::Numeric&               ) {}
    virtual void Visit( const parameters::ObjectKnowledge&       ) {}
    virtual void Visit( const parameters::Path&                  ) {}
    virtual void Visit( const parameters::PathPoint&             ) {}
    virtual void Visit( const parameters::PopulationKnowledge&   ) {}
    virtual void Visit( const parameters::Quantity&              ) {}
    virtual void Visit( const parameters::Resource&              ) {}
    virtual void Visit( const parameters::ResourceNetworkNode&   ) {}
    virtual void Visit( const parameters::ResourceNetworkType&   ) {}
    virtual void Visit( const parameters::String&                ) {}
    virtual void Visit( const parameters::UrbanBlock&            ) {}
    virtual void Visit( const Parameter_ABC&                     ) {}
    //@}
};

}

#endif // __ActionParameterVisitor_ABC_h_
