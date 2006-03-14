// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkDisplayer_h_
#define __LinkDisplayer_h_

#include "BaseDisplayer.h"
class ActionController;

// =============================================================================
/** @class  LinkDisplayer
    @brief  LinkDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class LinkDisplayer : public BaseDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LinkDisplayer( ActionController& actionController );
    virtual ~LinkDisplayer();
    //@}

    //! @name Operations
    //@{
    void Trigger( QString anchor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LinkDisplayer( const LinkDisplayer& );            //!< Copy constructor
    LinkDisplayer& operator=( const LinkDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const Agent& value );
    virtual void Call( const Population& value );
    virtual void Call( const Object_ABC& value );

    QString MakeLink( const QString& type, unsigned int id, const QString& name );
    int ParseLink( const QString& anchor, const QString& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    static QString link_;
    static QString agent_;
    static QString population_;
    static QString object_;
    ActionController& actionController_;
    //@}
};

#endif // __LinkDisplayer_h_
