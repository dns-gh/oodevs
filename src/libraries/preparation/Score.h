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

#include "Score_ABC.h"

namespace kernel
{
    class Controller;
}

namespace indicators
{
    class Variables;
    class Primitives;
    class ElementFactory_ABC;    
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Score
    @brief  Score
*/
// Created: SBO 2009-04-16
// =============================================================================
class Score : public Score_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Score( xml::xistream& xis, kernel::Controller& controller, const indicators::Primitives& indicators );
             Score( const QString& name, kernel::Controller& controller, const indicators::Primitives& indicators );
             Score( const QString& name, const QString& formula, const indicators::Variables& variables, const indicators::Primitives& indicators );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetFormula() const;
    virtual void Accept( indicators::VariablesVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    virtual void CheckValidity() const;
    virtual void Serialize( xml::xostream& xos ) const;
    Score& operator=( const Score& score );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score( const Score& );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    void SerializeIndicators( xml::xostream& xos ) const;
    void SerializeIndicator( xml::xostream& xos, const QString& formula ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller* controller_;
    QString name_;
    QString formula_;
    std::auto_ptr< indicators::Variables > variables_;
    std::auto_ptr< indicators::ElementFactory_ABC > elementFactory_;
    //@}
};

#endif // __Score_h_
