// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Urban_h_
#define __DEC_Knowledge_Urban_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "game_asn/Simulation.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

class DEC_Knowledge_UrbanPerception;
class MIL_Army_ABC;

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  DEC_Knowledge_Urban
    @brief  DEC_Knowledge_Urban
*/
// Created: MGD 2009-12-04
// =============================================================================
class DEC_Knowledge_Urban : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Urban( const MIL_Army_ABC& army, const urban::TerrainObject_ABC& object );
    virtual ~DEC_Knowledge_Urban();
    //@}
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    bool IsValid() const;
    void Prepare();
    void Update     ( const DEC_Knowledge_UrbanPerception& perception );
    void SetPerceptionLevel( const PHY_PerceptionLevel& level );
    bool Clean      () const;
    unsigned GetId  () const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    double GetProgress() const;
    //@}

    //! @name Relevance management
    //@{
    void     UpdateRelevance();
    MT_Float GetRelevance   () const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     ();
    void SendStateToNewClient();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Automate*, const PHY_PerceptionLevel* > T_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::iterator                             IT_PerceptionSourceMap;
    typedef T_PerceptionSourceMap::const_iterator                       CIT_PerceptionSourceMap;
    //@}

private:
    //! @name Tools
    //@{
    void ChangeRelevance   ( MT_Float rNewRelevance );
    void UpdatePerceptionSources( const DEC_Knowledge_UrbanPerception& perception );
    //@}

    //! @name Network operations
    //@{
    void WriteMsgPerceptionSources( ASN1T_MsgUrbanKnowledgeUpdate& asnMsg ) const;
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    void SendChangedState  ();
    void SendFullState     ();
    //@}

    //! @name CheckPoints
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_Knowledge_Urban* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_Knowledge_Urban* role, const unsigned int /*version*/ );
    //@}

private:
    const unsigned nID_;
    const MIL_Army_ABC& army_;
    const urban::TerrainObject_ABC& object_;

    // Internal attributes
    const PHY_PerceptionLevel*  pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*  pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel*  pMaxPerceptionLevel_;
          double                rProgressPercent_;
          double                rRelevance_;
          T_PerceptionSourceMap perceptionLevelPerAutomateMap_;

    // Network
    bool     bCreatedOnNetwork_;
    unsigned int nTimeLastUpdate_;
    bool     bLastPerceived_;
    bool     bRelevanceUpdated_;
    bool     bCurrentPerceptionLevelUpdated_;
    bool     bMaxPerceptionLevelUpdated_;
    double   rLastRelevanceSent_;
    double   rLastProgressSent_;
};

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::save_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void save_construct_data( Archive& archive, const DEC_Knowledge_Urban* knowledge, const unsigned int /*version*/ )
{
    const MIL_Army_ABC* const army = &knowledge->army_;
    const urban::TerrainObject_ABC* const object = &knowledge->object_;
    archive << army
            << object;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::load_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void load_construct_data( Archive& archive, DEC_Knowledge_Urban* knowledge, const unsigned int /*version*/ )
{
    MIL_Army_ABC* army;
    urban::TerrainObject_ABC* object;
    archive >> army
            >> object;
    ::new( knowledge )DEC_Knowledge_Urban( *army, *object );
}

#endif // __DEC_Knowledge_Urban_h_
