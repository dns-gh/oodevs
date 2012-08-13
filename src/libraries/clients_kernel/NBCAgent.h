// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAgent_h_
#define __NBCAgent_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  NBCAgent
    @brief  NBCAgent
*/
// Created: AGE 2006-04-04
// =============================================================================
class NBCAgent
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCAgent( xml::xistream& input );
    virtual ~NBCAgent();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    //void Update();
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    //@}
};

}

#endif // __NBCAgent_h_
