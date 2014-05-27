// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MissionController_h_
#define __MissionController_h_

#include "MissionController_ABC.h"
#include "MIL.h"

class MIL_AgentPion;

// =============================================================================
/** @class  MissionController
    @brief  Mission controller
*/
// Created: LGY 2011-06-14
// =============================================================================
class MissionController : public MissionController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionController( ActionManager& actions );
    virtual ~MissionController();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const tools::Resolver< MIL_AgentPion >& resolver, const PopulationFactory_ABC& populationFactory );
    virtual void SendFullState();
    virtual void Start( const boost::shared_ptr< MIL_Mission_ABC >& mission );
    virtual void Stop( const boost::shared_ptr< MIL_Mission_ABC >& mission );
    virtual uint32_t AcquireId();
    virtual ActionManager& GetActionManager() const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >        void load( Archive&, const unsigned int );
    template< typename Archive >        void save( Archive&, const unsigned int ) const;
    template< typename Archive > friend void save_construct_data( Archive&, const MissionController*, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive&, MissionController*, const unsigned int /*version*/ );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< MIL_Mission_ABC > T_Mission;
    typedef std::map< unsigned int, T_Mission >  T_Missions;
    //@}

private:
    //! @name Member data
    //@{
    ActionManager& actions_;
    T_Missions missions_;
    bool loaded_;
    uint32_t counter_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MissionController )

#endif // __MissionController_h_
