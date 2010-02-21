// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/entities/Population.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:05 $
// $Revision: 24 $
// $Workfile: Population.h $
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"
#include "Testable_Entity.h"

namespace DIN
{
    class DIN_Input;
}

namespace TEST
{
    class Workspace;
    class PopulationType;

// =============================================================================
// Created: SBO 2005-05-11
// =============================================================================
class Population : public Testable_Entity
{

private:

    class Concentration
    {
    public:
        //! @name Constructor/Destructor
        //@{
         Concentration( const MsgPopulationConcentrationCreation& asnMsg );
        ~Concentration();
        //@}

        //! @name Operations
        //@{
        void OnUpdate( const MsgPopulationConcentrationUpdate& asnMsg );
        //@} 

    public:
        Position position_;
    };

    typedef std::vector< Concentration* >         T_ConcentrationVector;
    typedef T_ConcentrationVector::const_iterator CIT_ConcentrationVector;

    class Flow
    {
    public:
        //! @name Constructor/Destructor
        //@{
         Flow( const MsgPopulationFlowCreation& asnMsg );
        ~Flow();
        //@}

        //! @name Operations
        //@{
        void OnUpdate( const MsgPopulationFlowUpdate& asnMsg );
        //@} 

    public:
        T_PositionVector shape_;
    };

    typedef std::vector< Flow* >         T_FlowVector;
    typedef T_FlowVector::const_iterator CIT_FlowVector;

public:
    //! @name Constructors/Destructor
    //@{
             Population( const Workspace& workspace, const MsgPopulationCreation& asnMsg );
    virtual ~Population();
    //@}

    //! @name Accessors
    //@{
    virtual       T_EntityId   GetId       () const;
    virtual const Position&    GetPosition () const;
	        const std::string& GetName     () const;
    //@}

    //! @name Messages handlers
    //@{
    void OnUpdate                ( const MsgPopulationUpdate&                   asnMsg );
    
    void OnConcentrationCreated  ( const MsgPopulationConcentrationCreation&    asnMsg );
    void OnConcentrationUpdated  ( const MsgPopulationConcentrationUpdate&      asnMsg );
    void OnConcentrationDestroyed( const MsgPopulationConcentrationDestruction& asnMsg );

    void OnFlowCreated           ( const MsgPopulationFlowCreation&             asnMsg );
    void OnFlowUpdated           ( const MsgPopulationFlowUpdate&               asnMsg );
    void OnFlowDestroyed         ( const MsgPopulationFlowDestruction&          asnMsg );
    //@}

    //! @name Operations
    //@{
    void SendMagicAction( int action ) const;
    //@}

    //! @name Mission Scheduling
    //@{
    void ScheduleAllMissions ( Scheduler& scheduler, unsigned int nIteration = 1 );
    void ScheduleMission     ( Scheduler& scheduler, const std::string& strMissionName, unsigned int nIteration = 1 );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId            nId_;
	std::string           strName_;
    const PopulationType* pType_;

    T_ConcentrationVector concentrations_;
    T_FlowVector          flows_;
    //@}
};

} // end namespace TEST

#include "Population.inl"

#endif // __Population_h_
