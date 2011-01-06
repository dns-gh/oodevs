// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ActionParameterSerializer_h_
#define __crossbow_ActionParameterSerializer_h_

#include <boost/shared_ptr.hpp>

namespace actions
{
    class Parameter_ABC;
}

namespace kernel
{
    class Location_ABC;
    class Entity_ABC;
    class OrderParameter;
    class CoordinateConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace plugins
{
namespace crossbow
{
    class OrderParameterTypeResolver;
    class Workspace_ABC;
    class Table_ABC;
    class Row_ABC;
    
// =============================================================================
/** @class  ActionParameterSerializer
    @brief  ActionParameterSerializer
*/
// Created: SBO 2007-05-31
// =============================================================================
class ActionParameterSerializer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterSerializer( Workspace_ABC& workspace, const kernel::CoordinateConverter_ABC& converter, const kernel::AgentKnowledgeConverter_ABC& agentConverter, const kernel::ObjectKnowledgeConverter_ABC& objectConverter, const kernel::EntityResolver_ABC& entities, kernel::Controller& controller );
    virtual ~ActionParameterSerializer();
    //@}

    //! @name Operations
    //@{    
    bool Serialize( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    bool Serialize( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterSerializer( const ActionParameterSerializer& );            //!< Copy constructor
    ActionParameterSerializer& operator=( const ActionParameterSerializer& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    void DoRegistration();
    bool DoSerialize( const std::string& type, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    bool DoSerialize( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    //@}

    //! @name Helpers
    //@{
    template< typename ParameterType, typename T >
    void SerializeRaw( const kernel::OrderParameter& parameter, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const;

    template< typename ParameterType >
    void SerializeId( const kernel::OrderParameter& parameter, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    template< typename ParameterType, typename ConverterType >
    void SerializeId( const kernel::OrderParameter& parameter, const std::string& value, const ConverterType& converter, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    
    void SerializeList( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& list ) const;
    void SerializeList( const kernel::OrderParameter& parameter, const std::string& value, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& list ) const;

    void SerializeLocation( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    template< typename ParameterType, typename GeometryType >
    void SerializeLocation( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    template< typename T, typename GeometryType >
    void SerializeLocationList( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    //@}

    //! @name 
    //@{
    void SerializeIntelligence( const kernel::OrderParameter& parameter, const std::string& value, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    void SerializePhaseLines( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& tablename, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    //@}

    //! @name 
    //@{
    void SerializeList( const kernel::OrderParameter& parameter, unsigned long parameterId, std::auto_ptr< actions::Parameter_ABC >& list ) const;
    //@}

private:

    class ParameterSerializerFactory;

private:
    //! @name Member data
    //@{
    Workspace_ABC& workspace_;  //!< Reference geometry database only
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::EntityResolver_ABC& entities_;
    const kernel::AgentKnowledgeConverter_ABC& agentConverter_;
    const kernel::ObjectKnowledgeConverter_ABC& objectConverter_;
    kernel::Controller& controller_;
    std::auto_ptr< ParameterSerializerFactory > factory_;
    //@}
};

}
}

#endif // __crossbow_ActionParameterSerializer_h_
