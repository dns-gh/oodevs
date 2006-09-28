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
    class Publisher_ABC;

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
    Side( Model& model, unsigned int nID, DIN::DIN_Input& msg );
    ~Side();
    //@}

    //! @name Accessors
    //@{
    ModelRefsContainer< KnowledgeGroup >& GetKnowledgeGroups();
    ModelRefsContainer< Automat        >& GetAutomats       ();
    unsigned long                         GetID             () const;
    //@}

    //! @name Main
    //@{
    void Update        ( const ASN1T_MsgChangeDiplomatie& asnMsg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< Side*, ASN1T_EnumDiplomatie > T_DiplomacyMap;
    typedef T_DiplomacyMap::const_iterator          CIT_DiplomacyMap;
    //@}

private:
          Model&                               model_;
    const unsigned long                        nID_;
          std::string                          strName_;
          ModelRefsContainer< KnowledgeGroup > knowledgeGroups_;
          ModelRefsContainer< Automat        > automats_;
          T_DiplomacyMap                       diplomacies_;
};

}

#include "Side.inl"

#endif // __Side_h_
