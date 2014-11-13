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

#include "MIL.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include <tools/Set.h>

class DEC_Decision_ABC;
class MIL_Agent_ABC;
class MIL_Population;
class TER_Localisation;

// =============================================================================
// @class  DEC_PopulationKnowledge
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationKnowledge : public DEC_KnowledgeResolver_ABC
{
public:
    //! @name Types
    //@{

    typedef std::vector< TER_Localisation > T_LocationVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_PopulationKnowledge( const MIL_Population& population );
    virtual ~DEC_PopulationKnowledge();
    //@}

    //! @name Main operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const MIL_Population& population ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( unsigned int nID ) const;
    //@}

    //! @name Operations
    //@{
    void NotifyAttackedBy( const MIL_Agent_ABC& attacker );
    std::vector< unsigned int > GetPionsAttacking() const;
    void NotifySecuredBy( const MIL_Agent_ABC& securer );
    std::vector< unsigned int > GetPionsSecuring() const;

    void NotifyChanneled( const TER_Localisation& location );
    const T_LocationVector& GetChannelingLocations() const;
    bool HasChannelingChanged() const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name DEC Functions
    //@{
	static std::vector< unsigned int > GetPionsAttacking( const DEC_Decision_ABC* agent );
	static std::vector< unsigned int > GetPionsSecuring( const DEC_Decision_ABC* agent );
    //@}

private:
    //! @name CheckPoints
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_PopulationKnowledge* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_PopulationKnowledge* role, const unsigned int /*version*/ );
    //@}

private:
    typedef tools::Set< const MIL_Agent_ABC* > T_AgentSet;

private:
    //! @name Member data
    //@{
    const MIL_Population& population_;
    T_AgentSet attackers_;
    T_AgentSet newAttackers_;
    T_AgentSet securers_;
    T_AgentSet newSecurers_;
    T_LocationVector channelingLocations_;
    T_LocationVector newChannelingLocations_;
    bool bChannelingChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_PopulationKnowledge )

#endif // __DEC_PopulationKnowledge_h_
