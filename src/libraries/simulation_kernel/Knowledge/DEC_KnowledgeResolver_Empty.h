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
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const;
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent ( const DIA_Variable_ABC&      dia ) const;
    virtual DEC_Knowledge_Agent*      ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const;
    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject( const DIA_Variable_ABC&      dia ) const;
    virtual DEC_Knowledge_Object*     ResolveKnowledgeObject(       uint                   nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const DIA_Variable_ABC&          dia ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_KnowledgeResolver_Empty( const DEC_KnowledgeResolver_Empty& );            //!< Copy constructor
    DEC_KnowledgeResolver_Empty& operator=( const DEC_KnowledgeResolver_Empty& ); //!< Assignement operator
    //@}

};

#endif // __DEC_KnowledgeResolver_Empty_h_
