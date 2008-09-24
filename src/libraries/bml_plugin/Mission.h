// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_h_
#define __Mission_h_

#include "game_asn/Simulation.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class MissionType;
    class OrderParameter;
}

namespace dispatcher
{
    class Model;
    class Automat;
    class Agent;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace bml
{
    class MissionParameterFactory;
    class MissionParameter_ABC;

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: SBO 2008-05-22
// =============================================================================
class Mission
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission( xml::xistream& xis, const dispatcher::Model& model );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    void Send( dispatcher::SimulationPublisher_ABC& publisher ) const;
    bool IsSet( const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mission( const Mission& );            //!< Copy constructor
    Mission& operator=( const Mission& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< MissionParameter_ABC* > T_Parameters;
    //@}

    //! @name Helpers
    //@{
    void SendAutomatMission( dispatcher::SimulationPublisher_ABC& publisher ) const;
    void SendAgentMission( dispatcher::SimulationPublisher_ABC& publisher ) const;

    void Serialize( ASN1T_MissionParameters& asn ) const;
    void Clean( ASN1T_MissionParameters& asn ) const;
    void SerializeDummyParameters( ASN1T_MissionParameters& asn ) const;
    void FillEmptyParameters( ASN1T_MissionParameters& asn ) const;
    void CleanDummyParameters( ASN1T_MissionParameters& asn ) const;
    
    const kernel::MissionType& ResolveMission( xml::xistream& xis );
    void AddParameter( MissionParameter_ABC& parameter );

    void ReadTaskeeWho( xml::xistream& xis );
    void ReadParameters( xml::xistream& xis );
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    const kernel::MissionType& type_;
    const dispatcher::Automat* automatTaskee_;
    const dispatcher::Agent* agentTaskee_;
    T_Parameters parameters_;
    std::auto_ptr< MissionParameterFactory > factory_;
    //@}
};

}
}

#endif // __Mission_h_
