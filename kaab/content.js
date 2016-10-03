function pushForRemoval(id, toRemove) {
	temp = document.getElementById(id);
	if(temp) {
		toRemove.push(temp);
	}
}

function removeRestrictions() {
	var toRemove = [];
	host = window.location.host
	switch(host) {
    case "www.leparisien.fr":
    	pushForRemoval("AB_overlay", toRemove);
        break;
    case "www.bricozone.fr":
	    pushForRemoval("adbcontainer-popup", toRemove);
	    pushForRemoval("adbpopup", toRemove);
        break;
	}
	// remove elements
	var removed = false
	for (i = 0; i < toRemove.length; i++) {
		removed = true
	    toRemove[i].parentNode.removeChild(toRemove[i]);
	}
	if(removed) {
		clearInterval(interval);
		chrome.runtime.sendMessage({removedElements: toRemove.length.toString()});
	}
	console.log("host: "+window.location.host)
}

chrome.runtime.sendMessage({removedElements: ""});
var interval = setInterval(removeRestrictions, 2000);