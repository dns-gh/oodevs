// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_ABC_h_
#define __LinkInterpreter_ABC_h_

#include "tools/Resolver_ABC.h"

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
    virtual bool InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LinkInterpreter_ABC( const LinkInterpreter_ABC& );            //!< Copy constructor
    LinkInterpreter_ABC& operator=( const LinkInterpreter_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool Interprete( const Q3Url& url );
    bool ExecuteCommand( const Q3Url& url );
    bool InterpreteId( const Q3Url& url );
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
