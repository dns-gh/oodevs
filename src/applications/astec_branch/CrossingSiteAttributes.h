// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttributes_h_
#define __CrossingSiteAttributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  CrossingSiteAttributes
    @brief  CrossingSiteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class CrossingSiteAttributes : public Extension_ABC
                             , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                             , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                             , public Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossingSiteAttributes( Controller& controller );
    virtual ~CrossingSiteAttributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSiteAttributes( const CrossingSiteAttributes& );            //!< Copy constructor
    CrossingSiteAttributes& operator=( const CrossingSiteAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    bool set_;
    unsigned int width_;
    unsigned int depth_;
    unsigned int speed_;
    bool         needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttributes_h_
