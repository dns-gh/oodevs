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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  NBCAttributes
    @brief  NBCAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class NBCAttributes : public kernel::NBCAttributes_ABC
                    , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCAttributes( kernel::PropertiesDictionary& dico );
             NBCAttributes( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents, kernel::PropertiesDictionary& dico );
    virtual ~NBCAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetAgent( const kernel::NBCAgent& agent );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCAttributes( const NBCAttributes& );            //!< Copy constructor
    NBCAttributes& operator=( const NBCAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    kernel::NBCAgent* nbc_;
    //@}
};

#endif // __NBCAttributes_h_
