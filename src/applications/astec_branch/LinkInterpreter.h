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

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
    // $$$$ AGE 2006-05-11: A un moment il va y avoir des risques rigolos d'injection
    // $$$$ AGE 2006-05-11: Quand un utilisateur avisé va appeler un objet cmd://format c:
*/
// Created: AGE 2006-05-11
// =============================================================================
class LinkInterpreter : public QObject
{   
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit LinkInterpreter( QObject* parent = 0 );
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
    //@}
};

#endif // __LinkInterpreter_h_
