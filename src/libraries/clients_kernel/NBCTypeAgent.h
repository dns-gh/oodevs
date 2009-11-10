// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCTypeAgent_h_
#define __NBCTypeAgent_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  NBCTypeAgent
    @brief  NBCTypeAgent
*/
// Created: RFT 2006-04-04
// =============================================================================
class NBCTypeAgent
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCTypeAgent( xml::xistream& input );
    virtual ~NBCTypeAgent();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    //@}
};

}

#endif // __NBCTypeAgent_h_
