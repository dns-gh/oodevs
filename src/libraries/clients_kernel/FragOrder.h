// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FragOrder_h_
#define __FragOrder_h_

namespace kernel
{
    class FragOrderType;

// =============================================================================
/** @class  FragOrder
    @brief  FragOrder
*/
// Created: AGE 2006-03-14
// =============================================================================
class FragOrder
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FragOrder( const FragOrderType& type );
    virtual ~FragOrder();
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    std::string GetName() const;
    const FragOrderType& GetType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FragOrder( const FragOrder& );            //!< Copy constructor
    FragOrder& operator=( const FragOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const FragOrderType& type_;
    //@}
};

}

#endif // __FragOrder_h_
