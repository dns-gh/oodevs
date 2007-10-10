// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionLinker_h_
#define __ActionLinker_h_

class Panel_ABC;

// =============================================================================
/** @class  ActionLinker
    @brief  ActionLinker
*/
// Created: AGE 2007-10-10
// =============================================================================
class ActionLinker
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionLinker();
    virtual ~ActionLinker();
    //@}

    //! @name Operations
    //@{
    void Register( const QString& action, Panel_ABC& panel );

    ActionLinker& Chain( const QString& action );
    ActionLinker& operator()( const QString& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionLinker( const ActionLinker& );            //!< Copy constructor
    ActionLinker& operator=( const ActionLinker& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::map< QString, Panel_ABC* > panels_;
    QString current_;
    //@}
};

#endif // __ActionLinker_h_
