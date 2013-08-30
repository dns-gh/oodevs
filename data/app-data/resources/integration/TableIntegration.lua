--- Return true if element is present in elements in teh table
-- @param t: table
-- @param k: the element
-- @author LMT
-- @release 2010-12-09
existsInside = function( t, k )
    for _,content in pairs( t ) do
        for _,element in pairs( content ) do
            if element == k then
                return true
            end
         end
    end
    return false
end

--- Return true if element is present in a table
-- @param t: table
-- @param k: the element
-- @author DDA
-- @release 2010-04-16
exists = function( t, k )
    for _,content in pairs( t ) do
        if content == k then
            return true
        end
    end
    return false
end

--- Return true if the index is valid
-- @param t: table
-- @param i: index
-- @author DDA
-- @release 2010-06-04
existsIndex = function( t, i )
    for index,_ in pairs( t ) do
        if index == i then
            return true
        end
    end
    return false
end

--- Return fusion table, veirify if elem is already in table
-- @param listl: first list
-- @param list2: the second one to add in the first one
-- @author GGE
-- @release 2010-08-20
fusionList = function( listl, list2 )
    local res = {}
    for _, elem in pairs( listl or emptyTable ) do
      res[#res+1] = elem
    end
    for _, elem in pairs( list2 or emptyTable ) do
        if not exists(res, elem) then
            res[#res+1] = elem
        end
    end
    return res
   
end

-- Return the returnList without the unit
-- @param unit : element to remove
-- @param returnList : list where we want to remove
-- @author LMT
-- @release 2012-01-31
removeFromList = function( unit, returnList )
    for i, elemListReturn in pairs( returnList ) do
        if unit == elemListReturn then
            table.remove( returnList, i )
            break
        end
    end
    return returnList
end

-- Return the returnList without the unit
-- @param list : list of elements to remove
-- @param returnList : list from which the elements must be removed
-- @author NMI
-- @release 2013-07-12
removeListFromList = function( list, returnList )
    for _, listElement in pairs( list ) do
        for i, elemListReturn in pairs( returnList ) do
            if listElement == elemListReturn then
                table.remove( returnList, i )
                break
            end
        end
    end
    return returnList
end

--- Returns a deep copy of the table given in parameter
-- @param tableFrom : list of elements to copy
copyTable =  function ( tableFrom ) 
    local tableTo = {}
    for k,v in pairs( tableFrom ) do
        tableTo[k] = v
    end
    return tableTo
end

--- Returns the size of the table given in parameter
-- @param table: the table
-- @return the size of the table
-- @author NMI
-- @release 2013-05-23
function tableSize ( table )
    local cpt = 0
    for _,k in pairs( table ) do
      cpt = cpt + 1
    end
    return cpt
end


--- Clears the table given in parameter
-- @param table: the table
-- @author NMI
-- @release 2013-05-24
function clearTable ( table )
    for k in pairs ( table ) do
        table[ k ] = nil
    end
end
