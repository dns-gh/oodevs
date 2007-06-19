// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FourStateOption_h_
#define __FourStateOption_h_

namespace kernel
{

// =============================================================================
/** @class  FourStateOption
    @brief  FourStateOption
*/
// Created: AGE 2007-05-31
// =============================================================================
class FourStateOption
{
public:
    //! @name Statics
    //@{
    static FourStateOption Off();
    static FourStateOption On();
    static FourStateOption Selected();
    static FourStateOption SuperiorSelected();

    static QString OffName();
    static QString OnName();
    static QString SelectedName();
    static QString SuperiorSelectedName();
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FourStateOption( const QString& state = OffName() );
    virtual ~FourStateOption();
    //@}

    //! @name Operations
    //@{
    bool IsSet( bool selected, bool superior ) const;
    bool operator==( const FourStateOption& rhs ) const { return rhs.state_ == state_; };
    operator QString() const;
    //@}

private:
    //! @name Member data
    //@{
    char state_;
    //@}
};

}

#endif // __FourStateOption_h_
