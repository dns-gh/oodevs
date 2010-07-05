// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeVisitor_ABC_h_
#define __KnowledgeVisitor_ABC_h_

class MIL_KnowledgeGroup;

// =============================================================================
/** @class  KnowledgeVisitor_ABC
    @brief  KnowledgeVisitor_ABC
*/
// Created: SLG 2010-04-07
// =============================================================================
class KnowledgeVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeVisitor_ABC(){}
    virtual ~KnowledgeVisitor_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void visit( const MIL_KnowledgeGroup& knowledgeGroup ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeVisitor_ABC( const KnowledgeVisitor_ABC& );            //!< Copy constructor
    KnowledgeVisitor_ABC& operator=( const KnowledgeVisitor_ABC& ); //!< Assignment operator
    //@}
};

#endif // __KnowledgeVisitor_ABC_h_
