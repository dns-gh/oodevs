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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  RotaAttributes
    @brief  RotaAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class RotaAttributes : public kernel::RotaAttributes_ABC
                     , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaAttributes();
             RotaAttributes( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents );
    virtual ~RotaAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDanger( unsigned int level );
    void AddAgent( const kernel::NBCAgent& agent );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaAttributes( const RotaAttributes& );            //!< Copy constructor
    RotaAttributes& operator=( const RotaAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadNbcAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::NBCAgent, QString >& nbcAgents );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_Nbcs;
    //@}

public:
    //! @name Member data
    //@{
    kernel::OptionalValue< unsigned int > danger_;
    T_Nbcs agents_;
    //@}
};

#endif // __RotaAttributes_h_
