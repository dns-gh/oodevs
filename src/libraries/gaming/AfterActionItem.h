// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItem_h_
#define __AfterActionItem_h_

namespace xml
{
    class xistream;
}
class AfterActionInput;
class AfterActionOutput;
class AfterActionItemBuilder_ABC;

// =============================================================================
/** @class  AfterActionItem
    @brief  AfterActionItem
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionItem
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItem( const std::string& type, xml::xistream& xis );
    virtual ~AfterActionItem();
    //@}

    //! @name Operations
    //@{
    void Build( AfterActionItemBuilder_ABC& builder ) const; 
    const std::string& GetType() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionItem( const AfterActionItem& );            //!< Copy constructor
    AfterActionItem& operator=( const AfterActionItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    const std::string name_;
    const std::string output_;
    const std::string input1_, input2_;
    const std::string paramName_, paramType_;
    //@}
};

#endif // __AfterActionItem_h_
