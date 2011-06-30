// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_UrbanPerception_h_
#define __DEC_Knowledge_UrbanPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Tools/MIL_IDManager.h"

class PHY_PerceptionLevel;

// =============================================================================
/** @class  DEC_Knowledge_UrbanPerception
    @brief  DEC_Knowledge_UrbanPerception
*/
// Created: MGD 2009-11-26
// =============================================================================
class DEC_Knowledge_UrbanPerception : public DEC_Knowledge_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_UrbanPerception( const MIL_Agent_ABC& agentPerceiving, unsigned int nUrbanObjectId );
    virtual ~DEC_Knowledge_UrbanPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const PHY_PerceptionLevel& perceptionLevel );
    void SetPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    bool Clean() const;
    //@}

    //! @name Accessor
    unsigned GetId() const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    unsigned int GetUrbanPerceivedId() const;
    const MIL_Agent_ABC& GetPerceiver() const;
    bool IsPerceived() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork();
    void SendStateToNewClient( unsigned int nCtx = 0 );
    //@}

private:
    //! @name CheckPoints
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_Knowledge_UrbanPerception* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_Knowledge_UrbanPerception* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned nID_;
    const MIL_Agent_ABC& perceiver_;
    unsigned int nUrbanObjectId_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_UrbanPerception )

#endif // __DEC_Knowledge_UrbanPerception_h_
