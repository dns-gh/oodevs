// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Federate.h $
// $Author: Age $
// $Modtime: 6/12/04 14:43 $
// $Revision: 14 $
// $Workfile: HLA_Federate.h $
//
// *****************************************************************************

#ifndef __HLA_Federate_h_
#define __HLA_Federate_h_

#include <hla/ObjectRegistration_ABC.h>

namespace hla
{
    template< typename ObjectClass > class Class;
    class ObjectIdentifier;
    class RtiAmbassador_ABC;
    class Federate;
}

class HLA_TimeFactory;
class HLA_TimeIntervalFactory;
class HLA_RoleInterface;
class HLA_Object_ABC;
class HLA_InteractionManager;

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_RealObject_ABC;

// =============================================================================
/** @class  HLA_Federate
    @brief  HLA federate facade
*/
// Created: AGE 2004-11-05
// =============================================================================
class HLA_Federate
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Federate( const std::string& strFederateName, unsigned int nTimeStepDuration );
    virtual ~HLA_Federate();
    //@}

    //! @name Operations
    //@{
    bool Join( const std::string& strFederationName );
    void Register  ( MIL_AgentPion& agent );
    void Register  ( MIL_RealObject_ABC& object );
    void Unregister( MIL_RealObject_ABC& object );

    void Step();
    void Tick();
    //@}

    //! @name Accessors
    //@{
    MIL_Agent_ABC*      GetLocalAgent ( const hla::ObjectIdentifier& objectId ) const;
    MIL_RealObject_ABC* GetLocalObject( const hla::ObjectIdentifier& objectId ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeClasses();
    //@}

    //! @name Copy/Assignement
    //@{
    HLA_Federate( const HLA_Federate& );            //!< Copy constructor
    HLA_Federate& operator=( const HLA_Federate& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< hla::ObjectIdentifier, MIL_Agent_ABC* > T_AgentMap;
    typedef T_AgentMap::const_iterator                      CIT_AgentMap;

    typedef std::map< hla::ObjectIdentifier, MIL_RealObject_ABC* > T_ObjectMap;
    typedef T_ObjectMap::const_iterator                          CIT_ObjectMap;
    //@}

private:
    //! @name Member data
    //@{
    HLA_TimeFactory*         pTimeFactory_;
    HLA_TimeIntervalFactory* pIntervalFactory_;
    hla::RtiAmbassador_ABC*  pAmbassador_;
    hla::Federate*           pFederate_;

    hla::Class< HLA_RoleInterface >*                  pUnitClass_;
    hla::ObjectRegistration_ABC< HLA_RoleInterface >* pUnitRegistration_;

    hla::Class< HLA_Object_ABC >*                    pObjectClass_;
    hla::ObjectRegistration_ABC< HLA_Object_ABC >*   pObjectRegistration_;

    HLA_InteractionManager*     pInteractionManager_;

    T_AgentMap  localAgents_;
    T_ObjectMap localObjects_;
    //@}
};

#endif // __HLA_Federate_h_
