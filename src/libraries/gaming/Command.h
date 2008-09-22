// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Command_h_
#define __Command_h_

// =============================================================================
/** @class  Command
    @brief  Command
*/
// Created: SBO 2008-07-02
// =============================================================================
class Command
{

public:
    //! @name Constructors/Destructor
    //@{
             Command( const std::string& from, const std::string& to, const std::string& text );
    virtual ~Command();
    //@}

    //! @name Accessors
    //@{
    std::string Source() const;
    std::string Target() const;
    std::string Name() const;
    std::string Argument( unsigned int index ) const;
    unsigned int ArgumentCount() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Command( const Command& );            //!< Copy constructor
    Command& operator=( const Command& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Parse( const std::string& text );
    //@}

private:
    //! @name Member data
    //@{
    std::string source_;
    std::string target_;
    std::vector< std::string > arguments_;
    //@}
};

#endif // __Command_h_
