--@TODO MGD Remove this ugly, find why scriptref error
function CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Point" ) then
    --default_engine.createPoint( {}, knowledgedPosition[1], knowledgedPosition[2], knowledgedPosition[3] )
    kn = knowledges.create( net.masagroup.sword.military.world.Point, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Area" ) then
    kn = knowledges.create( net.masagroup.sword.military.world.Area, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Object" ) then
    kn = knowledges.create( net.masagroup.sword.military.world.Object, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Agent" ) then
    kn = knowledges.create( net.masagroup.sword.military.world.Agent, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Automat" ) then
    kn = knowledges.create( net.masagroup.sword.military.world.Automat, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Population" ) then
    kn = knowledges.create( net.masagroup.sword.military.world.Population, knowledgeSource )
    kn.sim_pos = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] }
    return kn
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function InitTaskParameter( params, strKnowledgeType, name, knowledgeSource, knowledgedPosition, list )
  if list then
    params[name] = {};
    for i = 1, #knowledgeSource do
      params[name][i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
    end
  else
    params[name] = { CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition ) }
  end
end

function InitQueryReturn( params, strKnowledgeType, knowledgeSource, knowledgedPosition, list)
  if list then
    for i = 1, #knowledgeSource do
      params[i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
    end
  else
    params[1] = CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  end
end

function InitializeMe( strKnowledgeType, knowledgeSource, posx, posy ,posz )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Agent" ) then
    --default_engine.createMe( {}, x, y, z )
    --knowledges.initializeMe( strKnowledgeType, knowledgeSource )
    knowledges.me.sim_pos = { x=posx, y=posy, z=posz }
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function InitializeLeaderMe( strKnowledgeType, knowledgeSource )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Automat" ) then
    --knowledges.initializeMe( strKnowledgeType, knowledgeSource )
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end    



actionCallbacks = {}
actionKnowledgeCallbacks = {}

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

RemoveAction = function( action )
  actionCallbacks[ action ] = nil
  actionKnowledgeCallbacks[ action ] = nil
end

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