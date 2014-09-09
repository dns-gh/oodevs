// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KS_UrbanKnowledgeSynthetizer_h_
#define __DEC_KS_UrbanKnowledgeSynthetizer_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_Army;
class DEC_Knowledge_UrbanPerception;
class DEC_Knowledge_Urban;
class MIL_UrbanObject_ABC;

// =============================================================================
/** @class  DEC_Knowledge_Urban
    @brief  DEC_Knowledge_Urban
*/
// Created: MGD 2009-12-02
// =========================================================================
class DEC_KS_UrbanKnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_UrbanKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_Army& blackBoard );
             DEC_KS_UrbanKnowledgeSynthetizer();
    virtual ~DEC_KS_UrbanKnowledgeSynthetizer();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk( int currentTimeStep );
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    //@}

private:
    //! @name Update
    //@{
    void UpdateKnowledgesFromUrbanPerception( const DEC_Knowledge_UrbanPerception& perception );
    boost::shared_ptr< DEC_Knowledge_Urban > GetKnowledgeToUpdate( const MIL_UrbanObject_ABC& objectKnown ) const;
    void UpdateKnowledgeRelevance( boost::shared_ptr< DEC_Knowledge_Urban >& knowledge );
    void CleanKnowledgeUrban( boost::shared_ptr< DEC_Knowledge_Urban >& knowledge );
    //@}

    //! @name Operations
    //@{
    void SynthetizeSubordinatesPerception();
    void ProcessKnowledgesUrbanToForget ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< DEC_Knowledge_Urban > >  T_UrbanKnowledgeVector;
    //@}

private:
    //! @name Members
    //@{
    DEC_KnowledgeBlackBoard_Army* pBlackBoard_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_UrbanKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_UrbanKnowledgeSynthetizer::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
}

#endif // __DEC_KS_UrbanKnowledgeSynthetizer_h_
