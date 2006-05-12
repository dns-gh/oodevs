// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_h_
#define __LinkInterpreter_h_

#include "Resolver_ABC.h"

class Model;
class Controllers;

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
    // $$$$ AGE 2006-05-11: A un moment il va y avoir des risques rigolos d'injection
    // $$$$ AGE 2006-05-11: Quand un utilisateur avis� va appeler un objet cmd://format c:
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
    virtual bool Interprete( const QString& protocole, const QString& address );
    bool ExecuteCommand( const QString& address );
    bool FollowLink( const QString& address );
    bool InterpreteId( const QString& address );
    bool InterpreteId( const QString& classId, const QString& id );
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
