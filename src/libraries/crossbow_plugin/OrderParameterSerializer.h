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
    class Model;
}

namespace crossbow
{

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
    explicit OrderParameterSerializer( const dispatcher::Model& model );
    virtual ~OrderParameterSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( ASN1T_MissionParameter& asn, const kernel::OrderParameter& parameter, const std::string& value ) const;
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
    void SerializeLimit( ASN1T_Line*& asn, const std::string& value ) const;
    void SerializePhaseLines( ASN1T_LimasOrder*& asn, const std::string& value ) const;
    void SerializePhaseLine( ASN1T_LimaOrder& asn, const std::string& value ) const;
    void SerializeDirection( ASN1T_Heading& asn, const std::string& value ) const;
    void SerializeIntelligenceList( ASN1T_IntelligenceList*& asn, const std::string& value ) const;

    void SerializeAutomat( ASN1T_Automat& asn, const std::string& value ) const;
    void SerializeBool( ASN1BOOL& asn, const std::string& value ) const;
    void SerializeLocation( ASN1T_Location*& asn, const std::string& value ) const;
    void CleanLocation( ASN1T_Location*& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

}

#endif // __OrderParameterSerializer_h_
