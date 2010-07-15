// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CommandString_h_
#define __CommandString_h_

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  CommandString
    @brief  Command string
*/
// Created: SBO 2008-06-30
// =============================================================================
class CommandString
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CommandString( const std::string& message );
    virtual ~CommandString();
    //@}

    //! @name Operations
    //@{
    std::string Command() const;
    std::string Arg( unsigned int index ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CommandString( const CommandString& );            //!< Copy constructor
    CommandString& operator=( const CommandString& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > args_;
    //@}
};

}
}

#endif // __CommandString_h_
