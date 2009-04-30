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

#include "IndicatorDefinition_ABC.h"
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
class Score : public IndicatorDefinition_ABC
            , public kernel::Displayable_ABC
            
{

public:
    //! @name Constructors/Destructor
    //@{
             Score( const ASN1T_MsgIndicator& message, kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual std::string Commit( const T_Parameters& parameters ) const;
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

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    const QString name_;
    double value_;
    double tendency_;
    double normalized_;
    //@}
};

#endif // __Score_h_
