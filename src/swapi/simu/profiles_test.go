// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simu

import (
	. "launchpad.net/gocheck"
)

func (s *TestSuite) TestProfilesParsing(c *C) {
	xml := "" +
		`<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<profiles model-version="4.8.2" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="schemas/5.2.0/exercise/profiles.xsd">
  <profile name="admin" password="" supervision="true">
    <rights>
      <readonly/>
      <readwrite>
        <side id="1"/>
        <side id="2"/>
      </readwrite>
    </rights>
  </profile>
  <profile name="user1" password="user1" supervision="false">
    <rights>
      <readonly/>
      <readwrite>
        <side id="1"/>
      </readwrite>
    </rights>
  </profile>
  <profile name="user2" password="user2" supervision="false">
    <rights>
      <readonly/>
      <readwrite>
        <side id="2"/>
      </readwrite>
    </rights>
  </profile>
  <profile name="alluser" password="alluser" supervision="false">
    <rights>
      <readonly/>
      <readwrite>
        <side id="1"/>
        <side id="2"/>
      </readwrite>
    </rights>
  </profile>
</profiles>`
	profiles, err := ReadProfiles([]byte(xml))
	c.Assert(err, IsNil)
	c.Assert(profiles.Profiles, HasLen, 4)

	admin := profiles.Profiles[0]
	c.Assert(admin.Name, Equals, "admin")
	c.Assert(admin.Password, Equals, "")
	c.Assert(admin.Supervision, Equals, true)

	user2 := profiles.Profiles[2]
	c.Assert(user2.Name, Equals, "user2")
	c.Assert(user2.Password, Equals, "user2")
	c.Assert(user2.Supervision, Equals, false)

	su := profiles.FindSupervisor()
	c.Assert(su.Name, Equals, "admin")
}
