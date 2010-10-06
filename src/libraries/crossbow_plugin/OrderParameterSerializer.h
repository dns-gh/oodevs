// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderParameterSerializer_h_
#define __OrderParameterSerializer_h_

namespace Common
{
    class MsgMissionParameter;
    class ObjectKnowledgeId;
    class UnitKnowledgeId;
    class MsgIntelligenceList;
    class MsgHeading;
    class MsgLimasOrder;
    class MsgLimaOrder;
    class MsgMissionObjective;
    class MsgMissionObjectiveList;
    class UnitId;
    class MsgLine;
    class AutomatId;
    class MsgLocation;
    class MsgPointList;
}

namespace kernel
{
    class OrderParameter;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace plugins
{
namespace crossbow
{
    class OrderParameterTypeResolver;
    class Workspace_ABC;
    class Database_ABC;
    class Row_ABC;
    class Table_ABC;

// =============================================================================
/** @class  OrderParameterSerializer
    @brief  OrderParameterSerializer
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderParameterSerializer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameterSerializer( Workspace_ABC& database, const dispatcher::Model_ABC& model );
    virtual ~OrderParameterSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( Common::MsgMissionParameter& message, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value ) const;
    void Clean( Common::MsgMissionParameter& message ) const;
    bool IsValidParameter( const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderParameterSerializer( const OrderParameterSerializer& );            //!< Copy constructor
    OrderParameterSerializer& operator=( const OrderParameterSerializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializePhaseLines( Common::MsgLimasOrder& message, unsigned long parameterId, const std::string& tablename ) const;
    void SerializePhaseLine( Common::MsgLimaOrder& message, const std::string& value ) const;
    void SerializeDirection( Common::MsgHeading& message, const std::string& value ) const;
    void SerializeIntelligenceList( Common::MsgIntelligenceList& message, const std::string& value ) const;

    void SerializeUnit( Common::UnitId& message, const std::string& value ) const;
    void SerializeAutomat( Common::AutomatId& message, const std::string& value ) const;
    void SerializeUnitKnowledge( Common::UnitKnowledgeId& message, const std::string& value ) const;
    void SerializeObjectKnowledge( Common::ObjectKnowledgeId& message, const std::string& value ) const;
    void SerializeBool( bool& message, const std::string& value ) const;
    void SerializeMissionObjective( Common::MsgMissionObjective& message, const std::string& value ) const;
    void SerializeMissionObjectiveList( Common::MsgMissionObjectiveList& message, const std::string& value ) const;
    template< typename T >
    void SerializeLocation( T& message, unsigned long parameterId, const std::string& value ) const;
    template< typename T, typename Base >
    void SerializeLocationList( T& message, unsigned long parameterId, const std::string& value ) const;

    template< typename T >
    void SerializeLocation( T& message, const Row_ABC* row ) const;

    void CleanLocation( Common::MsgLocation*& message ) const;
    //@}

    //! @name Helper
    //@{
    void FillLocationlist( std::vector< Common::MsgLocation* >& locations, boost::shared_ptr< Table_ABC > table, unsigned long oid ) const;
    //@}

private:

    class LazyDatabaseConnection;

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
	Workspace_ABC& workspace_;  //!< Reference geometry database only
    std::auto_ptr< OrderParameterTypeResolver > types_;
    //@}
};

}
}

#endif // __OrderParameterSerializer_h_
