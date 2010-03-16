// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationKnowledge.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_PopulationKnowledge.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationKnowledge_h_
#define __DEC_PopulationKnowledge_h_

#include "Knowledge/DEC_Knowledge_Def.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

namespace Common
{
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
    class MsgUnitKnowledge;
}

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Population;

// =============================================================================
// @class  DEC_PopulationKnowledge
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationKnowledge : public DEC_KnowledgeResolver_ABC
                              , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Agent_ABC* > T_AgentSet;
    typedef T_AgentSet::const_iterator       CIT_AgentSet;

    typedef std::vector< TER_Localisation >  T_LocationVector;
    typedef T_LocationVector::const_iterator CIT_LocationVector;
    //@}

public:
    DEC_PopulationKnowledge( const MIL_Population& population );
    virtual ~DEC_PopulationKnowledge();

    //! @name Main operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       unsigned int                       nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban(       uint                   nID ) const;
    //@}

    //! @name Operations
    //@{
    void NotifyAttackedBy ( const MIL_Agent_ABC& attacker );
    std::vector< unsigned int > GetPionsAttacking( ) const;
    void NotifySecuredBy  ( const MIL_Agent_ABC& securer );
    std::vector< unsigned int > GetPionsSecuring ( ) const;
    
          void              NotifyChanneled       ( const TER_Localisation& location );
    const T_LocationVector& GetChannelingLocations() const;
          bool              HasChannelingChanged  () const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name CheckPoints
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_PopulationKnowledge* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_PopulationKnowledge* role, const unsigned int /*version*/ );
    //@}

private:
    const MIL_Population& population_;
    T_AgentSet attackers_; 
    T_AgentSet newAttackers_;

    T_AgentSet securers_;
    T_AgentSet newSecurers_;

    T_LocationVector channelingLocations_;
    T_LocationVector newChannelingLocations_;
    bool             bNewChannelingChanged_;
    bool             bChannelingChanged_;
};

BOOST_CLASS_EXPORT_KEY( DEC_PopulationKnowledge )

#endif // __DEC_PopulationKnowledge_h_
