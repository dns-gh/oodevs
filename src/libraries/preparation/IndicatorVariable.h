// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorVariable_h_
#define __IndicatorVariable_h_

#include "IndicatorElement_ABC.h"

class IndicatorType;

// =============================================================================
/** @class  IndicatorVariable
    @brief  IndicatorVariable
*/
// Created: SBO 2009-03-17
// =============================================================================
class IndicatorVariable : public IndicatorElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorVariable( const std::string& name, const std::string& type );
    virtual ~IndicatorVariable();
    //@}

    //! @name Accessors
    //@{
    virtual const IndicatorType& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< IndicatorElement_ABC > element );
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorVariable( const IndicatorVariable& );            //!< Copy constructor
    IndicatorVariable& operator=( const IndicatorVariable& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< IndicatorType > type_;
    //@}
};

#endif // __IndicatorVariable_h_
