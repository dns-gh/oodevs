require "lunit"

integration = {}
masalife = {}
masalife.brain = {}
masalife.brain.communication = {}
masalife.brain.integration = {}
masalife.brain.integration.createBrainLocalStorage = function() end
masalife.brain.knowledge = {}
masalife.brain.communication.protocol = {}
masalife.brain.communication.protocol.leaveGroup = {}
masalife.brain.communication.protocol.setDynamicCommFunID = function() end


dofile "../integration/ToolsFunctions.lua"


-- -------------------------------------------------------------------------------- 
-- Element activation or send a done feedback if no elements to activate
-- --------------------------------------------------------------------------------
module( "tables functions tests", package.seeall, lunit.testcase )

setup = function ()
end

-- The function existsIndex returns 'true' if the issued table is indexed with the issued value
existsIndex_TEST = function()
	local myTable = {}
	myTable = { myString = { "hello" }, "goodbye", true, false }
	local myIndex = 2
	local myResult = existsIndex( myTable, myIndex )
	assert_equal( true, myResult )

	myIndex = nil
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myIndex = "hello"
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myIndex = 1000
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myTable[ "myStringIndex1" ] = true
	myResult = existsIndex( myTable, "myStringIndex1" )
	assert_equal( true, myResult )

	myResult = existsIndex( myTable, "hey" )
	assert_equal( false, myResult )

	myTable[ true ] = "hoho"
	myResult = existsIndex( myTable, true )
	assert_equal( true, myResult )

	local myOtherTable = { string = "hello" }
	myTable[ myOtherTable ] = "goodbye"
	myResult = existsIndex( myTable, myOtherTable )
	assert_equal( true, myResult )

	myResult = existsIndex( myTable, myOtherTable.string )
	assert_equal( false, myResult )

end

-- Compare the issued value with elements of issued table.
exists_TEST = function()
	local myTable = { 1, 2, 3, 4 }
	local myValue = 2
	local result = exists( myTable, myValue )
	assert_equal( true, result )

	myTable = { myString = { "hello" }, "goodbye", true, false }
	myValue = "hello"
	result = exists( myTable, myValue )
	assert_equal( false, result  )

	myValue = true
	result = exists( myTable, myValue)
	assert_equal( true, result  )

	myValue = "goodbye"
	result = exists( myTable, myValue )
	assert_equal( true, result  )

	myValue = { "hello" } -- same value but not the same table
	result = exists( myTable, myValue )
	assert_equal( false, result  )

	result = exists( myTable, myTable.myString ) -- same table
	assert_equal( true, result  )

	result = exists( myTable, myTable[ 12 ] )
	assert_equal( false, result  )
	result = exists( myTable, myTable.nilValue )
	assert_equal( false, result  )

end



-- Test fusionList method. Fusion list method returns the fusionned table from two tables.
-- Only différents values are fusionned
fusionList_TEST = function()
	local filledListOK1 = { 1, 2, 3 }       -- size is 3
	local filledListOK2 = { "1", "2", "3" } -- size is 3
	local filledListOK3 = { 2, "hello",3, 6, true } -- size is 5, 2 value in common with filledListOK1
	local emptyList = {}
	local badIndexList = {}
	badIndexList[ "astring" ] = true
	badIndexList[ 2 ] = false
	badIndexList[ "trois" ] = "hey buddy!!"

	-- test 1
	local finalList = fusionList( filledListOK1, filledListOK2 )
	assert_equal(  #filledListOK1 + #filledListOK2, #finalList )

    -- test 2
	local finalList = fusionList( filledListOK1, filledListOK3 )
	assert_equal( 6, #finalList )

    -- test 3
	local finalList = fusionList( filledListOK1, emptyList )
	assert_equal( #filledListOK1, #finalList )

    -- test 4
	local finalList = fusionList( filledListOK2, filledListOK3 )
	assert_equal( #filledListOK1 + #filledListOK3, #finalList )

	-- test 5
	local finalList = fusionList( emptyList, filledListOK1 )
	assert_equal( #filledListOK1, #finalList )

    -- test 6
	local finalList = fusionList( emptyList, badIndexList )
	assert_equal( 3 , #finalList )

	assert_table( finalList )
end

-- The minValue function should returns the min value among numeric values of issued the tables
minValue_TEST = function()
	local myNumericTable = { 1, 56, 2, 32, 0 }
	local minVal = minValue( myNumericTable )
	assert_equal( 0, minVal )

	local myNumericTable = { 48, 56, 789, 32, 12 }
	local minVal = minValue( myNumericTable )
	assert_equal( 12, minVal )
end


teardown = function()
end