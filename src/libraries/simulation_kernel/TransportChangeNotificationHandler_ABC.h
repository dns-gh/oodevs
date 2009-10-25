// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __TransportChangeNotificationHandler_ABC_H__
#define __TransportChangeNotificationHandler_ABC_H__

namespace transport {

// =============================================================================
/** @class  TransportChangeNotificationHandler_ABC
    @brief  TransportChangeNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class TransportChangeNotificationHandler_ABC 
{
public:
	TransportChangeNotificationHandler_ABC();
	virtual ~TransportChangeNotificationHandler_ABC();

    virtual void NotifyIsLoadedForTransport() = 0;
    virtual void NotifyIsUnLoadedForTransport() = 0;
};

} // namespace transport

#endif /* __TransportChangeNotificationHandler_ABC_H__ */
