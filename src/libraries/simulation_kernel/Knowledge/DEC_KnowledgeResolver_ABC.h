// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_KnowledgeResolver_ABC_h_
#define __DEC_KnowledgeResolver_ABC_h_

namespace Common
{
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
    class MsgUnitKnowledge;
    class MsgUrbanKnowledge;
}

class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_Knowledge_Urban;

// =============================================================================
/** @class  DEC_KnowledgeResolver_ABC
    @brief  DEC_KnowledgeResolver_ABC
*/
// Created: NLD 2006-11-22
// =============================================================================
class DEC_KnowledgeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeResolver_ABC() {};
    virtual ~DEC_KnowledgeResolver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const Common::MsgUnitKnowledge& asn ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( unsigned int nID ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int nID ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_KnowledgeResolver_ABC( const DEC_KnowledgeResolver_ABC& );            //!< Copy constructor
    DEC_KnowledgeResolver_ABC& operator=( const DEC_KnowledgeResolver_ABC& ); //!< Assignment operator
    //@}
};

#endif // __DEC_KnowledgeResolver_ABC_h_
