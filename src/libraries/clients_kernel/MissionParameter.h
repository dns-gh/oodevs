// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_h_
#define __MissionParameter_h_

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  MissionParameter
    @brief  MissionParameter
*/
// Created: SBO 2007-04-19
// =============================================================================
class MissionParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter( xml::xistream& xis );
    virtual ~MissionParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    bool IsOptional() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter( const MissionParameter& );            //!< Copy constructor
    MissionParameter& operator=( const MissionParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString type_;
    bool optional_;
    //@}
};

}

#endif // __MissionParameter_h_
