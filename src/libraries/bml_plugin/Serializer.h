// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Serializer_h_
#define __Serializer_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Agent;
}

namespace xml
{
    class xostream;
}

namespace bml
{

// =============================================================================
/** @class  Serializer
    @brief  Serializer
*/
// Created: SBO 2008-05-16
// =============================================================================
class Serializer
{

public:
    //! @name Constructors/Destructor
    //@{
             Serializer();
    virtual ~Serializer();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgControlBeginTick& message );
    void SerializeReportingData( xml::xostream& xos, const dispatcher::Agent& entity ) const;
    void SerializeOrder( xml::xostream& xos, const ASN1T_MsgUnitOrder& message, const dispatcher::Agent& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Serializer( const Serializer& );            //!< Copy constructor
    Serializer& operator=( const Serializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SerializeTask( xml::xostream& xos, const ASN1T_MsgUnitOrder& message, const dispatcher::Agent& entity ) const;
    void SerializeAction( xml::xostream& xos, const ASN1T_MsgUnitOrder& message ) const;
    void SerializeControlMeasures( xml::xostream& xos, const ASN1T_MsgUnitOrder& message ) const;
    void SerializeControlMeasure( xml::xostream& xos, const ASN1T_MissionParameter& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string currentDate_;
    //@}
};

}

#endif // __Serializer_h_
