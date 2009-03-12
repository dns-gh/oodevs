// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Score_h_
#define __Score_h_

#include "clients_kernel/Displayable_ABC.h"
#include "game_asn/Aar.h"

namespace kernel
{
    class Controller;
}

class Publisher_ABC;

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-03-12
// =============================================================================
class Score : public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Score( const ASN1T_MsgIndicator& message, kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    void Update( const ASN1T_MsgIndicator& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score( const Score& );            //!< Copy constructor
    Score& operator=( const Score& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    QString name_;
    double value_;
    //@}
};

#endif // __Score_h_
