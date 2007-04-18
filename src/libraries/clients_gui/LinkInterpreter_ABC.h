// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_ABC_h_
#define __LinkInterpreter_ABC_h_

#include "clients_kernel/Resolver_ABC.h"


namespace kernel
{
    class ActionController;
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  LinkInterpreter_ABC
    @brief  Link interpreter base
*/
// Created: AGE 2006-05-11
// =============================================================================
class LinkInterpreter_ABC : public QObject
{   
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter_ABC( QObject* parent, kernel::ActionController& actions );
    virtual ~LinkInterpreter_ABC();
    //@}

    //! @name Slots
    //@{
public slots:
    virtual bool Interprete( const QString& link );
    //@}

protected:
    //! @name Operations
    //@{
    void AddEntity   ( const QString& category, const kernel::Entity_ABC& entity );
    void RemoveEntity( const QString& category, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LinkInterpreter_ABC( const LinkInterpreter_ABC& );            //!< Copy constructor
    LinkInterpreter_ABC& operator=( const LinkInterpreter_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool Interprete( const QUrl& url );
    bool ExecuteCommand( const QUrl& url );
    bool InterpreteId( const QUrl& url );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, const kernel::Entity_ABC* > T_Entities;
    typedef std::map< QString, T_Entities >                     T_TypedEntities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actions_;
    T_TypedEntities entites_;
    //@}
};

}

#endif // __LinkInterpreter_ABC_h_
