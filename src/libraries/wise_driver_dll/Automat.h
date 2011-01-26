// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "WiseEntity.h"

namespace sword
{
    class AutomatChangeKnowledgeGroup;
    class AutomatChangeSuperior;
}

namespace sword
{
    class AutomatAttributes;
    class AutomatCreation;
}

class Model;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: SEB 2010-10-13
// =============================================================================
class Automat : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const Model& model, const sword::AutomatCreation& message );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    virtual void Update( SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& attribute, const CWISEValueUnion& value );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatAttributes& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatChangeSuperior& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::AutomatChangeKnowledgeGroup& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const std::wstring name_;
    const unsigned long type_;
    const Entity_ABC* party_;
    const Entity_ABC* knowledgeGroup_;
    const Entity_ABC* superior_;
    //@}
};

#endif // __Automat_h_
