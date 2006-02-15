// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttributes_h_
#define __NBCAttributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  NBCAttributes
    @brief  NBCAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class NBCAttributes : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                    , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                    , public Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCAttributes();
    virtual ~NBCAttributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCAttributes( const NBCAttributes& );            //!< Copy constructor
    NBCAttributes& operator=( const NBCAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void Update( const ASN1T_MsgObjectUpdate& message );
    virtual void Update( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void DoUpdate( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    bool set_;
    unsigned long nbcId_; // $$$$ AGE 2006-02-14: resolve
    //@}
};

#endif // __NBCAttributes_h_
