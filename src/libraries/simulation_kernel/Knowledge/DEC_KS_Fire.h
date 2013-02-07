// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.h $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 5 $
// $Workfile: DEC_KS_Fire.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Fire_h_
#define __DEC_KS_Fire_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_AgentPion;
class MIL_AgentPion;
class MIL_Population;

// =============================================================================
/** @class  DEC_KS_Fire
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to manage
            the fires relatives knowledges
                - direct fire   : detection of the attacker
                - indirect fire : fire information (source / dest) when looking
                                  at a zone where the flying ammos pass through
                - explosion     : explosion information (object concerned)
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Fire : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_Fire( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_Fire();
    virtual ~DEC_KS_Fire();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Clean();
    virtual void Talk( int currentTimeStep );
    bool IsAttacked() const;
    //@}

    //! @name Events
    //@{
    void NotifyAttackedBy( MIL_AgentPion&  attacker );
    void NotifyAttackedBy( MIL_Population& attacker );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Types
    //@{
    struct PionComparator {
        bool operator() (const MIL_AgentPion* lhs, const MIL_AgentPion* rhs) const;
    };
    typedef std::set< MIL_AgentPion*, PionComparator >  T_PionSet;
    typedef T_PionSet::const_iterator   CIT_PionSet;

    struct PopulationComparator {
        bool operator() (const MIL_Population* lhs, const MIL_Population* rhs) const;
    };
    typedef std::set< MIL_Population*, PopulationComparator >     T_PopulationSet;
    typedef T_PopulationSet::const_iterator CIT_PopulationSet;
    //@}

private:
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    T_PionSet                          pionsAttacking_;
    T_PopulationSet                    populationsAttacking_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_Fire )

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_Fire::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

#endif // __DEC_KS_Fire_h_
