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

#include <tools/Resolver_ABC.h>

class QUrl;

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
    void AddEntity   ( const std::string& category, const kernel::Entity_ABC& entity );
    void RemoveEntity( const std::string& category, const kernel::Entity_ABC& entity );
    virtual bool InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool Interprete( const QUrl& url );
    bool ExecuteCommand( const QUrl& url );
    bool InterpreteId( const QUrl& url );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, const kernel::Entity_ABC* > T_Entities;
    typedef std::map< std::string, T_Entities >                     T_TypedEntities;
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
