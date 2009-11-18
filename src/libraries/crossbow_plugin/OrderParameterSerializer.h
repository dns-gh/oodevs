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

#include "game_asn/Simulation.h"

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
    namespace tools
    {
        class OrderParameterTypeResolver;
    }
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
    explicit OrderParameterSerializer( Database_ABC& database, const dispatcher::Model_ABC& model );
    virtual ~OrderParameterSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( ASN1T_MissionParameter& asn, const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& value ) const;
    void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderParameterSerializer( const OrderParameterSerializer& );            //!< Copy constructor
    OrderParameterSerializer& operator=( const OrderParameterSerializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializePhaseLines( ASN1T_LimasOrder*& asn, unsigned long parameterId, const std::string& tablename ) const;
    void SerializePhaseLine( ASN1T_LimaOrder& asn, const std::string& value ) const;
    void SerializeDirection( ASN1T_Heading& asn, const std::string& value ) const;
    void SerializeIntelligenceList( ASN1T_IntelligenceList*& asn, const std::string& value ) const;

    void SerializeUnit( ASN1T_Unit& asn, const std::string& value ) const;
    void SerializeAutomat( ASN1T_Automat& asn, const std::string& value ) const;
    void SerializeUnitKnowledge( ASN1T_UnitKnowledge& asn, const std::string& value ) const;
    void SerializeObjectKnowledge( ASN1T_ObjectKnowledge& asn, const std::string& value ) const;
    void SerializeBool( ASN1BOOL& asn, const std::string& value ) const;
    void SerializeMissionObjective( ASN1T_MissionObjective*& asn, const std::string& value ) const;
    void SerializeMissionObjectiveList( ASN1T_MissionObjectiveList*& asn, const std::string& value ) const;
    template< typename T >
    void SerializeLocation( T*& asn, unsigned long parameterId, const std::string& value ) const;
    template< typename T >
    void SerializeLocationList( T*& asn, unsigned long parameterId, const std::string& value ) const;
    template< typename T >
    void SerializeLocation( T*& asn, const Row_ABC* row ) const;
    //@}

    //! @name Helper
    //@{
    void FillLocationlist( std::vector< ASN1T_Location* >& locations, boost::shared_ptr< Table_ABC > table, unsigned long oid ) const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    Database_ABC& database_;
    std::auto_ptr< tools::OrderParameterTypeResolver > types_;
    //@}
};

}
}

#endif // __OrderParameterSerializer_h_
