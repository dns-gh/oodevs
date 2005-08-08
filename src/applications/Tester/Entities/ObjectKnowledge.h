// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Team.h"

namespace TEST {

    class Object;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
    @par    Using example
    @code
    ObjectKnowledge;
    @endcode
*/
// Created: SBO 2005-08-08
// =============================================================================
class ObjectKnowledge
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg, const Team& team );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Accessors
    //@{
    T_EntityId GetId() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId  nId_;
    const Team& owner_;
    Object*     pRealObject_;
    //@}
};

} // end namespace TEST

#include "ObjectKnowledge.inl"

#endif // __ObjectKnowledge_h_
