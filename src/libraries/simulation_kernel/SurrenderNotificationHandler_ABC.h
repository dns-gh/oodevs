/*
 * SurrenderNotificationHandler_ABC.h
 *
 *  Created on: 28 sept. 2009
 *      Author: ahc
 */

#ifndef __surrender_SurrenderNotificationHandler_ABC_H__
#define __surrender_SurrenderNotificationHandler_ABC_H__

namespace surrender {

class SurrenderNotificationHandler_ABC {
public:
	SurrenderNotificationHandler_ABC();
	virtual ~SurrenderNotificationHandler_ABC();

	virtual void NotifyCaptured() =0;
	virtual void NotifyReleased() =0;
};

}

#endif /* __surrender_SurrenderNotificationHandler_ABC_H__ */
