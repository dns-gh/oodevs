// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaAttributes_h_
#define __RotaAttributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  RotaAttributes
    @brief  RotaAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class RotaAttributes : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaAttributes();
    virtual ~RotaAttributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaAttributes( const RotaAttributes& );            //!< Copy constructor
    RotaAttributes& operator=( const RotaAttributes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_NbcIds;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    bool set_;
    unsigned int danger_;
    T_NbcIds agents_;
    //@}
};

#endif // __RotaAttributes_h_
