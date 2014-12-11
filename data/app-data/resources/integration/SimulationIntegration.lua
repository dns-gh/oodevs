---\ Knowledge management /---

require 'debug'

-- optimization
emptyTable = masalife.brain.core.constants.empty

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
  integration.stopTasks() --Stop all tasks, main task and fragOrder
end )

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

--- Create a knowledge
-- @param strKnowledgeType The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source object
-- @author MGD
-- @release 2010-01-22
function CreateKnowledge( knowledgeType, knowledgeSource, params )
    local result = localStorageVar.knowledgeCache[ knowledgeSource ]
    if result then return result end
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
    if list then
        if #knowledgeSource > 0 then
            local currentIndex = #params[name]+1
            params[name][currentIndex] = {};
            local currentParam = params[name][currentIndex] 
            for i = 1, #knowledgeSource do
              currentParam[#currentParam+1] = CreateKnowledge( strKnowledgeType, knowledgeSource[i] )
            end
        end
    else
        params[name][#params[name]+1] = CreateKnowledge( strKnowledgeType, knowledgeSource )
    end
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
    reportFunction = integration.pionRC
    RegisterTaskListener()
    RegisterDoneTaskListener()
    InitKnowledgeManager()
end

function InitMeCrowd( strKnowledgeType, knowledgeSource, groupName )
    masalife.brain.knowledge.me.body = CreateKnowledge( strKnowledgeType, knowledgeSource )
    meKnowledge = masalife.brain.knowledge.me.body
    myself.engineerObjectsOnPath = {}
    reportFunction = integration.crowdRC
    RegisterTaskListener()
    InitKnowledgeManager()
end

function InitMeAutomat( strKnowledgeType, knowledgeSource, groupName )
    masalife.brain.knowledge.me.body = CreateKnowledge( strKnowledgeType, knowledgeSource )
    meKnowledge = masalife.brain.knowledge.me.body
    myself.reportMessages = {}
    reportFunction = integration.genericRC
    RegisterTaskListener()
    InitKnowledgeManager()
end

--- Displays a given report. If this method is called during multiple
-- consecutive ticks with the same "instance" parameter (and the same report), 
-- then the report will only be displayed the first time this method was called.
-- @param instance Any type, the parameter used to determine whether or not to
-- display reports multiple times over multiple calls.
-- Any additional parameter will be used as parameters to the report.
reportOnceAction = masalife.brain.integration.startStopAction( { 
    start = function( instance, ... ) 
        reportFunction( ... )
    end
} )

--- Displays a given report. If this method is called during multiple
-- consecutive ticks by this entity with the same report, then the
-- report will only be displayed the first time this method was called.
-- Any additional parameter will be used as parameters to the report.
reportOnceFunction = function( ... )
    reportOnceAction( myself, ... )
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

local swordErrorHandler = function( err )
    DEC_Trace( tostring( err )..debug.traceback() )
    return err
end

installErrorHandler = function()
    errorhandlermanager.installErrorHandler( swordErrorHandler )
end

CallbackMovingOnPath = function( position )
    if myself.callbackMovingOnPath then
        myself.callbackMovingOnPath( position )
    end
end

-- Brain debugging

local enabledTraces = {} -- tracing state for each agent (true: set; nil: unset)

function ActivateBrainDebug( activate )
    local state = activate or nil
    if enabledTraces[ myself ] ~= state then -- needs to toggle state?
        masalife.brain.core.getModelData( -- list all skills
            {
                NotifySkill = function( self, package, name )
                    masalife.brain.core.setTracing( activate, package, "select" )
                end
            } )
        enabledTraces[ myself ] = state
    end
end

integration.getMaxInt = function()
    return 2147483647
end