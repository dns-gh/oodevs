var elements = new Array();

module.exports.GetElements = function() {
    return elements
}

module.exports.GetElement = function(index) {
    if ( index >= 0 && index < elements.length)
        return elements[index]
    else
        return null
}

module.exports.GetLastElement = function() {
    return elements[elements.length-1];
}

module.exports.GetLastElements = function(num) {
    var last = new Array()
    for (i = elements.length - 1; i >= elements.length - num && i >=0 ; --i) { 
        last.push(elements[i]);
    }
    return last;
}

module.exports.PushElement = function(elem) {
    elements.push(elem)
}