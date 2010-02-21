// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_ABC_h_
#define __MissionParameter_ABC_h_

namespace Common
{
    class MsgMissionParameter;
}

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_ABC
    @brief  MissionParameter_ABC
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_ABC
{
public:
    //! @name Statics
    //@{
    static MissionParameter_ABC* Create( const Common::MsgMissionParameter& asn );
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_ABC( const Common::MsgMissionParameter& asn );
    virtual ~MissionParameter_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Send  ( Common::MsgMissionParameter& asn ) const = 0;
    virtual void Delete( Common::MsgMissionParameter& asn ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_ABC( const MissionParameter_ABC& );            //!< Copy constructor
    MissionParameter_ABC& operator=( const MissionParameter_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    bool bNullValue_;
    //@}
};

}

#endif // __MissionParameter_ABC_h_
