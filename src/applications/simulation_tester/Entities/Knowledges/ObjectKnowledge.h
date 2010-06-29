// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#include "Types.h"
#include "Entities/Team.h"

namespace TEST {

    class Object;
    class EntityManager;

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
             ObjectKnowledge( const EntityManager&                    entityManager,
                              const MsgObjectKnowledgeCreation& asnMsg,
                              const Team&                             team );
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
