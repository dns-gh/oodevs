// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgesVisitor_ABC_h_
#define __KnowledgesVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  KnowledgesVisitor_ABC
    @brief  Knowledges visitor declaration
*/
// Created: LGY 2011-08-29
// =============================================================================
class KnowledgesVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgesVisitor_ABC() {}
    virtual ~KnowledgesVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void VisitKnowledgesAgent( std::size_t knowledges ) = 0;
    virtual void VisitKnowledgesObject( std::size_t knowledges ) = 0;
    virtual void VisitKnowledgesPopulation( std::size_t knowledges ) = 0;
    //@}
};

#endif // __KnowledgesVisitor_ABC_h_
