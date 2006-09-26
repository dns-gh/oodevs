// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Side_h_
#define __Side_h_

#include "AsnTypes.h"
#include "ModelRefsContainer.h"

namespace dispatcher
{
    class Dispatcher;
    class KnowledgeGroup;
    class Automat;
    class Model;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: NLD 2006-09-19
// =============================================================================
class Side
{
public:
    //! @name Constructors/Destructor
    //@{
    Side( Model& model, unsigned int nID, DIN::DIN_Input& dinMsg );
    ~Side();
    //@}

    //! @name Accessors
    //@{
    ModelRefsContainer< KnowledgeGroup >& GetKnowledgeGroups();
    ModelRefsContainer< Automat        >& GetAutomats       ();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignement operator
    //@}

private:
    const unsigned long                        nID_;
          std::string                          strName_;
          ModelRefsContainer< KnowledgeGroup > knowledgeGroups_;
          ModelRefsContainer< Automat        > automats_;
};

}

#include "Side.inl"

#endif // __Side_h_
