// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Karma_h_
#define __Karma_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
    class Team_ABC;

// =============================================================================
/** @class  Karma
    @brief  Karma
*/
// Created: SBO 2007-02-26
// =============================================================================
class Karma
{
public:
    static const Karma friend_;
    static const Karma enemy_;
    static const Karma neutral_;
    static const Karma unknown_;

    //! @name Constructors/Destructor
    //@{
             Karma();
    virtual ~Karma();
    //@}

    //! @name Operations
    //@{
    const std::string& GetId() const;
    unsigned int GetUId() const;
    QString GetName() const;

    const Karma& RelativeTo( const Karma& rhs ) const;

    const Karma& operator!() const;
    bool operator<( const Karma& rhs ) const;
    bool operator==( const Karma& rhs ) const;

    static const Karma& ResolveId  ( const std::string& id );
    static const Karma& ResolveName( const QString& name );
    //@}

private:
    //! @name Helpers
    //@{
    Karma( const unsigned int id, const std::string& identifier );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int uId_;
    std::string identifier_;
    //@}
};

}

#endif // __Karma_h_
