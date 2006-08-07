// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_h_
#define __LinkInterpreter_h_

#include "astec_kernel/Resolver_ABC.h"

class Model;
class Controllers;

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
*/
// Created: AGE 2006-05-11
// =============================================================================
class LinkInterpreter : public QObject
{   
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter( QObject* parent, Controllers& controllers, Model& model );
    virtual ~LinkInterpreter();
    //@}

    //! @name Slots
    //@{
public slots:
    virtual bool Interprete( const QString& link );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LinkInterpreter( const LinkInterpreter& );            //!< Copy constructor
    LinkInterpreter& operator=( const LinkInterpreter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool Interprete( const QUrl& url );
    bool ExecuteCommand( const QUrl& url );
    bool InterpreteId( const QUrl& url );
    template< typename T >
    bool Activate( const Resolver_ABC< T >& resolver, unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Model& model_;
    //@}
};

#endif // __LinkInterpreter_h_
