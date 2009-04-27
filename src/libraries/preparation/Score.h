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

namespace xml
{
    class xistream;
}

class IndicatorVariables;
class IndicatorPrimitives;
class IndicatorElementFactory_ABC;

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
             Score( xml::xistream& xis, kernel::Controller& controller, const IndicatorPrimitives& indicators );
             Score( const QString& name, kernel::Controller& controller, const IndicatorPrimitives& indicators );
             Score( const QString& name, const QString& formula, const IndicatorVariables& variables, const IndicatorPrimitives& indicators );
    virtual ~Score();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetFormula() const;
    virtual void Accept( IndicatorVariablesVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
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
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller* controller_;
    QString name_;
    QString formula_;
    std::auto_ptr< IndicatorVariables > variables_;
    std::auto_ptr< IndicatorElementFactory_ABC > elementFactory_;
    //@}
};

#endif // __Score_h_
