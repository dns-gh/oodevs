// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolRequest_h_
#define __SymbolRequest_h_

class AgentNature;

// =============================================================================
/** @class  SymbolRequest
    @brief  Symbol request
*/
// Created: SBO 2006-03-20
// =============================================================================
class SymbolRequest
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolRequest( const AgentNature& nature );
    virtual ~SymbolRequest();
    //@}

    //! @name Operations
    //@{
    bool Matches( const std::string& name, const std::string& value ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SymbolRequest( const SymbolRequest& );            //!< Copy constructor
    SymbolRequest& operator=( const SymbolRequest& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const AgentNature& nature_;
    //@}
};

#endif // __SymbolRequest_h_
