// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "AsnTypes.h"
#include "ModelsContainer.h"

namespace dispatcher
{
class Dispatcher;
class Side;
class KnowledgeGroup;
class Automat;
class Agent;    
class Object;
class KnowledgeAgent;
class SimulationModel;
class Publisher_ABC;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: NLD 2006-09-19
// =============================================================================
class Model
{
public:
    //! @name Constructors/Destructor
    //@{
     Model( Dispatcher& dispatcher );
    ~Model();
    //@}

    //! @name Update
    //@{
    void Update( const ASN1T_MsgsOutSim& asnMsg );
    void Update( uint nMsgID, DIN::DIN_Input& input );

    void Send  ( Publisher_ABC& publisher ) const;
    //@}

    //! @name Accessors
    //@{
          ModelsContainer< Side           >& GetSides          ();
          ModelsContainer< KnowledgeGroup >& GetKnowledgeGroups();
          ModelsContainer< Automat        >& GetAutomats       ();
          ModelsContainer< Agent          >& GetAgents         ();
    const ModelsContainer< Agent          >& GetAgents         () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    void OnReceiveMsgCreateSide          ( DIN::DIN_Input& msg );
    void OnReceiveMsgCreateKnowledgeGroup( DIN::DIN_Input& msg );
    //@}

private:
    Dispatcher&         dispatcher_;
    SimulationModel*    pSimulationModel_;

    ModelsContainer< Side >           sides_;
    ModelsContainer< KnowledgeGroup > knowledgeGroups_;
    ModelsContainer< Automat        > automats_;
    ModelsContainer< Agent          > agents_;
    ModelsContainer< Object         > objects_;
    ModelsContainer< KnowledgeAgent > knowledgesAgent_;
};

}

#include "Model.inl"

#endif // __Model_h_
