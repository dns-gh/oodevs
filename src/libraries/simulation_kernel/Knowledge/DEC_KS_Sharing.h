// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Sharing.h $
// $Author: Nld $
// $Modtime: 20/04/05 19:13 $
// $Revision: 3 $
// $Workfile: DEC_KS_Sharing.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Sharing_h_
#define __DEC_KS_Sharing_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  DEC_KS_Sharing
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Sharing : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_Sharing( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard );
             DEC_KS_Sharing();
    virtual ~DEC_KS_Sharing();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    virtual void Talk( int currentTimeStep );

    void ShareFromSource( const boost::shared_ptr< MIL_KnowledgeGroup >& source, unsigned int nShareTimeStep );
    void ShareFromSource( const boost::shared_ptr< MIL_KnowledgeGroup >& source, unsigned int nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

public:
    //! @name Types
    //@{
    struct sShareSource
    {
        sShareSource();
        sShareSource( const boost::shared_ptr< MIL_KnowledgeGroup >& shareSource );
        sShareSource( const boost::shared_ptr< MIL_KnowledgeGroup >& shareSource, const MT_Vector2D& vSharedCircleCenter, double rSharedCircleRadius );

        boost::shared_ptr< MIL_KnowledgeGroup > pShareSource_;
        MT_Vector2D vSharedCircleCenter_;
        double rSharedCircleRadius_;
    };

    typedef std::multimap< unsigned int /*nShareTimeStep*/, sShareSource > T_ShareSourceMMap;
    //@}

private:
    DEC_KnowledgeBlackBoard_KnowledgeGroup* pBlackBoard_;
    T_ShareSourceMMap shareSources_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_Sharing )

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KS_Sharing::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_Sharing::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
}

#endif // __DEC_KS_Sharing_h_
