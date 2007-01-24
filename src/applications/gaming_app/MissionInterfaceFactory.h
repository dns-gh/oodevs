// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterfaceFactory_h_
#define __MissionInterfaceFactory_h_

#include "clients_kernel/Resolver.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "network/Simulation_Asn.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
}

namespace xml
{
    class xistream;
}

class MissionTemplate;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionInterfaceFactory
    @brief  Mission interface factory
*/
// Created: SBO 2006-11-22
// =============================================================================
class MissionInterfaceFactory : public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterfaceFactory( kernel::Controllers& controllers );
    virtual ~MissionInterfaceFactory();
    //@}

    //! @name Operations
    //@{
    void CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgAutomateOrder&   order );
    void CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgPionOrder&       order );
    void CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgPopulationOrder& order );
    void CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MsgFragOrder&       order );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionInterfaceFactory( const MissionInterfaceFactory& );            //!< Copy constructor
    MissionInterfaceFactory& operator=( const MissionInterfaceFactory& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, MissionTemplate* > T_MissionTemplates;
    typedef T_MissionTemplates::const_iterator        CIT_MissionTemplates;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& message );
    void ReadMission( xml::xistream& xis, T_MissionTemplates& missions );
    bool CreateMissionInterface( MissionInterfaceBuilder& builder, unsigned long id, ASN1T_MissionParameters& parameters, T_MissionTemplates& missions );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_MissionTemplates   automatMissions_;
    T_MissionTemplates   unitMissions_;
    T_MissionTemplates   populationMissions_;
    T_MissionTemplates   fragOrders_;
    //@}
};

#endif // __MissionInterfaceFactory_h_
