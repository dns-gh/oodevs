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

namespace Common
{
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
    class MsgUnitKnowledge;
}

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
             DEC_KnowledgeBlackBoard_Automate();
    explicit DEC_KnowledgeBlackBoard_Automate( MIL_Automate& automate );
    virtual ~DEC_KnowledgeBlackBoard_Automate();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       unsigned int               nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       unsigned int                nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       unsigned int                    nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban(       unsigned int            nID ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Automate* pAutomate_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_Automate )

#endif // __DEC_KnowledgeBlackBoard_Automate_h_
