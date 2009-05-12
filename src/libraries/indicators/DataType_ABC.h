// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DataType_ABC_h_
#define __DataType_ABC_h_

namespace indicators
{

// =============================================================================
/** @class  DataType_ABC
    @brief  DataType_ABC
*/
// Created: SBO 2009-05-11
// =============================================================================
class DataType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DataType_ABC() {}
    virtual ~DataType_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve() const = 0;
    virtual void AddParameter( const DataType_ABC& definition, const DataType_ABC& instance ) = 0;
    virtual std::string ToString() const = 0;
    virtual bool operator==( const DataType_ABC& rhs ) const = 0;
    virtual bool operator!=( const DataType_ABC& rhs ) const = 0;
    virtual bool IsCompatible( const std::string& type ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DataType_ABC( const DataType_ABC& );            //!< Copy constructor
    DataType_ABC& operator=( const DataType_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __DataType_ABC_h_
