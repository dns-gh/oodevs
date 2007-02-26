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

namespace kernel
{

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
             Karma( const std::string& identifier, const QString& name );
    virtual ~Karma();
    //@}

    //! @name Operations
    //@{
    std::string GetId() const;
    QString GetName() const;

    bool operator<( const Karma& rhs ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string identifier_;
    QString name_;
    //@}
};

}

#endif // __Karma_h_
