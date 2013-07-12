---\ Knowledge management /---

require 'debug'

emptyTable = {} -- optimization

--- Bind DIA4 event interface to DIA5 interface


rawset( getfenv(), "StartEvent", function( task, params )
  if not ( task == "BEH_Defaut" ) then -- MGD Avoid BEH_Defaut Exception for DIA5
    myself.taskParams = params
    myself.newTask = true
    myself.currentMission = task
    masalife.brain.core.startTask( task, params )
  end
end )

rawset( getfenv(), "StopEvents", function( ... )
  masalife.brain.core.stopTasks() --Stop all tasks, main task and fragOrder
  myself.currentMission = nil
  myself.taskParams = {}
end )

function knowledgeIdFunction( knowledge )
    return tostring( knowledge.source )
end

masalife.brain.communication.protocol.setDynamicCommFunID( knowledgeIdFunction )

--- Create a knowledge
-- @param strKnowledgeType The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source object
-- @author MGD
-- @release 2010-01-22
function CreateKnowledge( knowledgeType, knowledgeSource, params )
    return CreateKnowledgeWithoutKB( knowledgeType, knowledgeSource, params )
    --return CreateKnowledgeInKB( knowledgeType, knowledgeSource, params )
end

function CreateKnowledgeInKB( knowledgeType, knowledgeSource, params )
    local result =  masalife.brain.knowledge.create( knowledgeType, knowledgeSource, params )
    result.source = knowledgeSource
    return result
end

--[[
-- Knowledge objects must remain the same from one tick to the next in order for DirectIA to work properly.
-- However, all knowledge objects should not remain in memory during a 7 day exercise, as most of them are only of transient use.
-- Therefore they must be kept in memory for a while, typically the length of a mission, and be allowed to be cleared at some point.
-- The easiest is to allow the garbage collector to reclaim them after a tick is finished.
-- knowledgeCache is a weak table that is used to return existing knowledges.
-- persistentKnowledgeCache makes sure the knowledge objects remain in memory until the end of the tick.
-- It is up to the user to keep the knowledge in memory (typically inside self.someparameter of their node).
]]
localStorageVar = masalife.brain.integration.createBrainLocalStorage( 
    function()
        return {
            knowledgeCache = setmetatable( {}, {  __mode ="kv",
                __newindex = function( t, k, v ) local hash = getmetatable( k )        rawset( t, (type( hash ) == "function" and hash( k )) or k, v ) end;
                __index    = function( t, k )    local hash = getmetatable( k ) return rawget( t, (type( hash ) == "function" and hash( k )) or k ) end
            } ),
            persistentKnowledgeCache = setmetatable( {}, {
                __newindex = function( t, k, v ) local hash = getmetatable( k )        rawset( t, (type( hash ) == "function" and hash( k )) or k, v ) end;
                __index    = function( t, k )    local hash = getmetatable( k ) return rawget( t, (type( hash ) == "function" and hash( k )) or k ) end
            } )
        }
    end 
)
-- The index stuff is needed for shared pointer objects management

local totalNumberOfKnowledgesAcrossTheWorld = 0

function CreateKnowledgeWithoutKB( knowledgeType, knowledgeSource, params )
    local result = localStorageVar.knowledgeCache[ knowledgeSource ]
    if result then return result end
    masalife.brain.knowledge.getSource = function( body )
        return knowledgeSource
    end
    result = knowledgeType:new()
    result.source = knowledgeSource
    if params then
        for i, j in pairs( params ) do
            result[i] = j
        end
    end
    localStorageVar.knowledgeCache[ knowledgeSource ] = result
    localStorageVar.persistentKnowledgeCache[ knowledgeSource ] = result
    return result
end

--- Create a proxy knowledge
-- @param strKnowledgeType The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source object
-- @author MGD
-- @release 2010-04-07
function CreateProxyKnowledge( strKnowledgeType, knowledgeSource, params )
    knowledgeFactory = knowledgeFactory or {}
    if not params then
      params = {proxy = knowledgeSource, distanceMin = 0, distanceMax = 200 }
    else
      params.proxy = knowledgeSource
    end
    if not knowledgeFactory[ knowledgeSource ] then
        knowledgeFactory[ knowledgeSource ] = {}
    end
    if not knowledgeFactory[ knowledgeSource ][ strKnowledgeType ] then
       knowledgeFactory[ knowledgeSource ][ strKnowledgeType ] = {}
       knowledgeFactory[ knowledgeSource ][ strKnowledgeType ].knowledge = CreateKnowledge( strKnowledgeType, knowledgeFactory[ knowledgeSource ][ strKnowledgeType ], params )
    else
        for i, j in pairs( params ) do
            knowledgeFactory[ knowledgeSource ][ strKnowledgeType ].knowledge[i] = j
        end
    end
    return knowledgeFactory[ knowledgeSource ][ strKnowledgeType ].knowledge
end

InitTaskParameterList = {}
InitTaskParameterList[ "beginlist" ] = function( params, name )
    params[name] = {}
    InitTaskParameter = AppendTaskParameterFunction
end
InitTaskParameterList[ "endlist" ] = function( params, name )
    InitTaskParameter = InitTaskParameterFunction
end

--- Fill mission parameters and create corresponding knowledges
-- @param params The table to fill
-- @param strKnowledgeType  The concret knowledge type to instanciate for the parameter
-- @param name The parameter name
-- @param knowledgeSource  The cpp source objects
-- @param knowledgedPosition The knowledges' current positions
-- @param list Indicate if the parameter is a list
-- @author MGD
-- @release 2010-01-22
function InitTaskParameterFunction( params, strKnowledgeType, name, knowledgeSource, list )
    if not knowledgeSource then
        InitTaskParameterList[ strKnowledgeType ]( params, name )
        return
    end
    if list then
        if #knowledgeSource > 0 then
            params[name] = {};
            for i = 1, #knowledgeSource do
              params[name][i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i] )
            end
        end
    else
        params[name] = CreateKnowledge( strKnowledgeType, knowledgeSource )
    end
end

function AppendTaskParameterFunction( params, strKnowledgeType, name, knowledgeSource, list )
    if not knowledgeSource then
        InitTaskParameterList[ strKnowledgeType ]( params, name )
        return
    end
    params[name][#params[name]+1] = CreateKnowledge( strKnowledgeType, knowledgeSource )
end

InitTaskParameter = InitTaskParameterFunction

--- Fill queries result and create corresponding knowledges
-- @param params The table to fill
-- @param strKnowledgeType  The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source objects
-- @param knowledgedPosition The knowledges' current positions
-- @param list Indicate if the parameter is a list
-- @author MGD
-- @release 2010-01-22
function InitQueryReturn( params, strKnowledgeType, knowledgeSource, list)
  if list then
    for i = 1, #knowledgeSource do
      params[ #params + 1 ] = CreateKnowledge( strKnowledgeType, knowledgeSource[i] )
    end
  else
    params[1] = CreateKnowledge( strKnowledgeType, knowledgeSource )
  end
end

--- Init body and groupe
-- @param strKnowledgeType The brain's role type
-- @param knowledgeSource  The cpp source object
-- @param groupName The name of the leader
-- @author MGD
-- @release 2010-02-25
function InitMePlatoon( strKnowledgeType, knowledgeSource, groupName )
    masalife.brain.knowledge.me.body = CreateKnowledge( strKnowledgeType, knowledgeSource )
    meKnowledge = masalife.brain.knowledge.me.body
    myself.engineerObjectsOnPath = {}
    myself.enemyToEngage = {{}, {}}
    RegisterTaskListener()
    RegisterDoneTaskListener()
    InitKnowledgeManager()
end

function InitMeCrowd( strKnowledgeType, knowledgeSource, groupName )
    masalife.brain.knowledge.me.body = CreateKnowledge( strKnowledgeType, knowledgeSource )
    meKnowledge = masalife.brain.knowledge.me.body
    myself.engineerObjectsOnPath = {}
    RegisterTaskListener()
    InitKnowledgeManager()
end

function InitMeAutomat( strKnowledgeType, knowledgeSource, groupName )
    masalife.brain.knowledge.me.body = CreateKnowledge( strKnowledgeType, knowledgeSource )
    meKnowledge = masalife.brain.knowledge.me.body
    myself.reportMessages = {}
    RegisterTaskListener()
    InitKnowledgeManager()
end

--- Unregister brain from communication group before deletion
-- @param groupName The name of the leader
-- @author MGD
-- @release 2010-01-22
function CleanBrainBeforeDeletion( groupName )
    masalife.brain.communication.protocol.leaveGroup( groupName )
end

---\ Callback management /---

actionCallbacks = {}
actionKnowledgeCallbacks = {}
perceptionReconnaissanceCallbacks = {}

CallbackAction = function( action, value )
  if actionCallbacks[ action ] then
      actionCallbacks[ action ]( value )
  end
end

KnowledgeCallbackAction = function( action, value )
  if actionKnowledgeCallbacks[ action ] then
      actionKnowledgeCallbacks[ action ]( value )
  end
end

CallbackPerception = function( id )
    if perceptionReconnaissanceCallbacks[ id ] then
        perceptionReconnaissanceCallbacks[ id ]()
    end
end

RemoveAction = function( action )
  actionCallbacks[ action ] = nil
  actionKnowledgeCallbacks[ action ] = nil
end

---\Tools /---


--- Return a linear interpolation between to point
-- @param min The min value on Y axis
-- @param min The max value on Y axis
-- @param start The value on x where interpolation begin
-- @param stop The value on x where interpolation stop
-- @param upslop Indicate if the function inscrease or decrease
-- @param value The x value where we want f(x)
-- @author MGD
-- @release 2010-01-22
LinearInterpolation = function( min, max, start, stop, upslope, value )
    local res = 0;
    if( upslope ) then
        if( value < start ) then
            res = min
        elseif( value > stop ) then
            res = max
        else
            res = min + ( value - start ) * ( max - min ) / ( stop - start )
        end
    else
        if( value < start ) then
            res = max
        elseif( value > stop ) then
            res = min
        else
            res = max + ( value - start ) * ( min - max ) / ( stop - start )
        end
    end
    return res
end

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

--- Return min value in table
-- @param t: table of numeric values
-- @author DDA
-- @release 2010-11-26
minValue = function( t )
    local minValue = math.huge
    for i, _ in pairs( t ) do
        if t[ i ] < minValue then
            minValue =  t[ i ]
        end
    end
    return minValue
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

-- Return the returnList without element from list
-- @param list : element to remove
-- @param returnList : list where we want to remove
-- @author LMT
-- @release 2011-01-13
removeFromListForLead = function( list, returnList )
    for _, listElement in pairs( list ) do
        for i, elemListReturn in pairs( returnList ) do
            if listElement.entity == elemListReturn then
                table.remove( returnList, i )
                break
            end
        end
    end
    return returnList
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

-- Return the list without element from list
removeListElementWithSameLocation = function( position, list )
    local result = {}
    for i, elemListReturn in pairs( list ) do
        if position ~= elemListReturn:getPosition() then
            result[#result+1] = elemListReturn
        end
    end
    return result
end

--- retourne la chaine de caractère passée en paramètre découpée selon le séparateur
-- @param separator: le séparateur
-- @param str: la chaine de caractère
-- @author LMT
-- @release 2010-11-18
explode =  function ( seperator, str ) 
    local pos, arr = 0, {}
    for st, sp in function() return string.find( str, seperator, pos, true ) end do
        arr[#arr+1] = string.sub( str, pos, st-1 )
        pos = sp + 1
    end
    arr[#arr+1] = string.sub( str, pos )
    return arr
end


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

--- Compare two elements with their efficiency
-- @param w1 First element
-- @param w2 Second element
-- @return True if the efficiency of w1 is better than w2's
-- @author MGD
-- @release 2010-12-03
local function comp( w1,w2 )
    return ( w1.efficiency > w2.efficiency )
end

--- Fill the unit task parameter
-- @param bestList Units sorted by efficiency
-- @param companyTask The company's task
-- @param params Parameters from the company's task
-- @param nbrFront The number of element that we want in this echelon
-- @author MGD
-- @release 2010-12-03
fillParameters = function( bestList, companyTask, params, nbrFront, context, isMain, objectif )
    local bestListNbrFront = {}
    local findFront = 1
    local current = 1
    params.maxNbrFront = math.min( #bestList, nbrFront )
    while findFront <= nbrFront and current <= params.maxNbrFront do
       local paramsPion = bestList[current].task:fillParameters( companyTask, params, bestList[current].entity, context, objectif, bestList[current].taskName ) -- Save the parameter needed for the platoon task
       if paramsPion then
          if isMain then          
              F_Pion_SeteEtatEchelon( bestList[current].entity.source, eEtatEchelon_First )
          end   
          bestListNbrFront[findFront] = bestList[current]
          bestListNbrFront[findFront].params = paramsPion
          findFront = findFront+1
       end
       current = current+1
    end
    return bestListNbrFront
end

-- @param entities A list of PlatoonAlly
-- @param tasks A list of Task that we want to give
-- @param companyTask The company's task
-- @param effect The effect of the company's task
-- @param params Parameters from the company's task
-- @param isMain True if tasks are for the first echelon
-- @return List of entities than can do tasks, sorted by efficiency
-- @author MGD
-- @modifier LMT
-- @release 2010-12-03
findBests = function( entities, tasks, companyTask , params, nbrFront, context, isMain, objectif )
    local bestList = {}
    local size = 1
    local bestTask = nil
    local bestTaskName = nil
    local bestParams = nil
    for _, entity in pairs( entities ) do
        local efficiencyFind = false
        local bestEfficiency = -1
        local efficiency  = -1
        local nTasks = #tasks
        for i = 1, nTasks do
            local taskName = tasks[i]
            local task = integration.RetrievePionTask( entity, taskName ) -- Save the task object if  the entity can do it
            if task then
                efficiency = entity:computePhysicalEfficiencyForEffect( task:getPionEfficiency() ) -- Efficiency for this entity to do this task
                if efficiency > bestEfficiency then
                    efficiencyFind = true
                    bestEfficiency = efficiency
                    bestTask = task
                    bestTaskName = taskName
                end
            end
        end
        if efficiencyFind then
            bestList[size] = { entity = entity,
                               task = bestTask,
                               taskName = bestTaskName,
                               efficiency = bestEfficiency }
            size = size + 1
        end
    end
    table.sort( bestList, comp ) -- Sort the list in order to have the couple entity/task with the best efficiency in first
    if isMain then
        pcall( function() return companyTask:distributeObjectives( bestList, params ) end )
        myself.leadData.nbrWithMainTask = #bestList
    end
    return fillParameters( bestList, companyTask, params, nbrFront, context, isMain, objectif )
end

-- @param entities A list of Company
-- @param tasks A list of Task that we want to give
-- @param companyTask The batallion's task
-- @param params Parameters from the batallion's task
-- @author LMT
-- @release 2011-12-15
fillParametersAutomat = function( bestList, companyTask, params, nbrFront )
    local bestListNbrFront = {}
    local findFront = 1
    local current = 1
    params.maxNbrFront = math.min( #bestList, nbrFront )
    while findFront <= nbrFront and current <= params.maxNbrFront do
       local paramsPion = bestList[current].task:fillParameters( companyTask, params, bestList[current].entity ) -- Save the parameter needed for the platoon task
       if paramsPion then
          bestListNbrFront[findFront] = bestList[current]
          bestListNbrFront[findFront].params = paramsPion
          findFront = findFront+1
       end
       current = current+1
    end
    return bestListNbrFront
end

-- @param entities A list of Company
-- @param tasks A list of Task that we want to give
-- @param companyTask The batallion's task
-- @param params Parameters from the batallion's task
-- @author LMT
-- @release 2011-12-15
findBestsAutomat = function( entities, tasks, companyTask , params, nbrFront )
    local bestList = {}
    local size = 1
    for _, entity in pairs( entities ) do
        local nTasks = #tasks
        for i = 1, nTasks do
            local taskName = tasks[i]
            local task = integration.RetrieveAutomatInBatallionTask( entity, taskName ) -- Save the task object if  the entity can do it
            if task then
                bestList[size] = { entity = entity,
                               task = task,
                               taskName = taskName }
                size = size + 1
            end
        end
    end
    return fillParametersAutomat( bestList, companyTask, params, nbrFront )
end

affichePositions = function( elements )
    local simPoints = {}
    for _, element in pairs( elements ) do
        local point = element:getPosition()
        simPoints[ #simPoints + 1 ] = point
    end
    DEC_DebugAffichePoints( simPoints )
end

waitInMin = function( self, delay )
    self[myself] = self[myself] or {}
    self[myself].tempsDebut = self[myself].tempsDebut or getSimulationTime()
    return  delay * 60 <= getSimulationTime() - self[myself].tempsDebut
end

getBetterReachingEfficiencyInList = function( elements, entity )
    local nPosition = #elements
    local bestPosition = -1
    local currentPositionReach = -1	
    local bestReturnPosition = nil
    for i = 1, nPosition do
        local position = elements[i]
        currentPositionReach = position:reachEfficiency( entity )
        local distance = integration.distance( position, entity)
        if currentPositionReach > bestPosition then
            bestPosition = currentPositionReach
            bestReturnPosition = position
        end
    end
    return bestReturnPosition
end

startSettleCalcul = function()
    local listePions = integration.getAgentsWithHQ()
    return DEC_Geometrie_StartCalculLignesAvantEtArrierePourPion( DEC_GetAutomate( meKnowledge.source ), listePions )
end

settleDistance = function( line )
    return DEC_Geometrie_CalculerDistanceLigneAvant( line, meKnowledge.source )
end

stopSettleCalcul = function( line )
    DEC_Geometrie_StopCalculLignesAvantEtArriere( line )
end

getSimulationTime = function()
    return DEC_TempsSim()
end

local swordErrorHandler = function( err )
    DEC_Trace( tostring( err )..debug.traceback() )
    return err
end

installErrorHandler = function()
    errorhandlermanager.installErrorHandler( swordErrorHandler )
end

--
-- Return a random number between minValue and maxValue, maxValue excluded
--
getRandomNumber = function( minValue, maxValue )
    return DEC_RandomValue( minValue, maxValue )
end