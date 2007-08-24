// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderType_h_
#define __OrderType_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class OrderParameter;

// =============================================================================
/** @class  OrderType
    @brief  OrderType
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderType( xml::xistream& xis );
    virtual ~OrderType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    unsigned int GetParameterCount() const;
    const OrderParameter* FindParameter( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderType( const OrderType& );            //!< Copy constructor
    OrderType& operator=( const OrderType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const OrderParameter* > T_Parameters;
    typedef T_Parameters::const_iterator                 CIT_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __OrderType_h_
