// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InternalLinks_h_
#define __InternalLinks_h_

class Agent;
class Object;
class Population;

// =============================================================================
/** @class  InternalLinks
    @brief  InternalLinks
*/
// Created: AGE 2006-05-11
// =============================================================================
class InternalLinks
{
public:
    //! @name Constants
    //@{
    const static QString agent_;
    const static QString object_;
    const static QString population_;
    //@}

public:
    //! @name Operations
    //@{
    static QString CreateLink( const Agent& entity, const QString& message );
    static QString CreateLink( const Object& entity, const QString& message );
    static QString CreateLink( const Population& entity, const QString& message );

    static QString CreateLink( const QString& type, unsigned long id, const QString& message );
    //@}

private:
    //! @name Helpers
    //@{
    static QString Embed( const QString& message, const QString& classId, unsigned long id );
    //@}
};

#endif // __InternalLinks_h_
