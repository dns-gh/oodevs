// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ResultsFactory_h_
#define __ResultsFactory_h_

#include "ElementFactory_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
}

// =============================================================================
/** @class  ResultsFactory
    @brief  ResultsFactory
*/
// Created: AGE 2008-08-04
// =============================================================================
class ResultsFactory : public ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ResultsFactory( dispatcher::ClientPublisher_ABC& publisher, int context = 0 );
    virtual ~ResultsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResultsFactory( const ResultsFactory& );            //!< Copy constructor
    ResultsFactory& operator=( const ResultsFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Result( const std::string& name, xml::xistream& xis, Task& result ) const;
    static void Error( const std::string& name );
    struct ResultDispatcher;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    int context_;
    //@}
};

#endif // __ResultsFactory_h_
