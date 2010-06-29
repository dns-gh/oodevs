// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Karma_h_
#define __Karma_h_

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
    static Karma friend_;
    static Karma enemy_;
    static Karma neutral_;
    static Karma unknown_;

    //! @name Constructors/Destructor
    //@{
             Karma();
    virtual ~Karma();
    //@}

    //! @name Operations
    //@{
    std::string GetId() const;
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
    Karma( const unsigned int id, const std::string& identifier, const QString& name );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int uId_;
    std::string identifier_;
    QString name_;
    //@}
};

}

#endif // __Karma_h_
