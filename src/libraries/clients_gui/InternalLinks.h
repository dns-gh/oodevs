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

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  InternalLinks
    @brief  InternalLinks
*/
// Created: AGE 2006-05-11
// =============================================================================
class InternalLinks
{
public:
    //! @name Operations
    //@{
    static QString CreateLink( const kernel::Entity_ABC& entity, const QString& message );
    static QString CreateLink( const QString& type, unsigned long id, const QString& message );
    //@}

private:
    //! @name Helpers
    //@{
    static QString Embed( const QString& message, const QString& classId, unsigned long id );
    //@}
};

}

#endif // __InternalLinks_h_
