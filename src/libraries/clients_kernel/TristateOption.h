// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TristateOption_h_
#define __TristateOption_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{

// =============================================================================
/** @class  TristateOption
    @brief  TristateOption
*/
// Created: AGE 2006-03-30
// =============================================================================
class TristateOption
{
public:
    //! @name Constants
    //@{
    static TristateOption Off();
    static TristateOption On();
    static TristateOption Auto();

    static QString OffName();
    static QString OnName();
    static QString AutoName();
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit TristateOption( const QString& state = OffName() );
    virtual ~TristateOption();
    //@}

    //! @name Operations
    //@{
    bool IsSet( bool automaticCondition ) const;
    bool operator==( const TristateOption& rhs ) const { return rhs.state_ == state_; };
    operator QString() const;
    //@}

private:
    //! @name Member data
    //@{
    char state_;
    //@}
};

}

#endif // __TristateOption_h_
