// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_KnowledgeResolver_Empty_h_
#define __DEC_KnowledgeResolver_Empty_h_

#include "MIL.h"
#include "DEC_KnowledgeResolver_ABC.h"

namespace Common
{
    class MsgUnitKnowledge;
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
    class MsgUrbanKnowledge;
}

// =============================================================================
/** @class  DEC_KnowledgeResolver_Empty
    @brief  DEC_KnowledgeResolver_Empty
*/
// Created: NLD 2006-11-22
// =============================================================================
class DEC_KnowledgeResolver_Empty : public DEC_KnowledgeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeResolver_Empty();
    virtual ~DEC_KnowledgeResolver_Empty();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( unsigned int nID ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_KnowledgeResolver_Empty( const DEC_KnowledgeResolver_Empty& );            //!< Copy constructor
    DEC_KnowledgeResolver_Empty& operator=( const DEC_KnowledgeResolver_Empty& ); //!< Assignment operator
    //@}
};

#endif // __DEC_KnowledgeResolver_Empty_h_
