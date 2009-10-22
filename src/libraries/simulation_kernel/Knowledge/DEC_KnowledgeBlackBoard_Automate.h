// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Automate.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard_Automate.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_Automate_h_
#define __DEC_KnowledgeBlackBoard_Automate_h_

#include "DEC_KnowledgeBlackBoard_ABC.h"

class MIL_Automate;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_Automate
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_Automate : public DEC_KnowledgeBlackBoard_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeBlackBoard_Automate( MIL_Automate& automate );
             DEC_KnowledgeBlackBoard_Automate();
    virtual ~DEC_KnowledgeBlackBoard_Automate();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const;
    //@}

private:
    MIL_Automate* pAutomate_;
};

#endif // __DEC_KnowledgeBlackBoard_Automate_h_
