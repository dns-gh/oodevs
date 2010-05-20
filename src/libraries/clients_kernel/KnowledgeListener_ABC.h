// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeListener_ABC_h_
#define __KnowledgeListener_ABC_h_

namespace kernel {

// =============================================================================
/** @class  KnowledgeListener_ABC
    @brief  KnowledgeListener_ABC
*/
// Created: JSR 2010-05-19
// =============================================================================
class KnowledgeListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeListener_ABC() {};
    virtual ~KnowledgeListener_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void NotifyDestruction() = 0;
    //@}

};

} // end namespace kernel

#endif // __KnowledgeListener_ABC_h_
