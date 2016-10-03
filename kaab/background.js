chrome.browserAction.setBadgeBackgroundColor({ color: [0,0,0,255] });
chrome.browserAction.setBadgeText({text: ''});

chrome.runtime.onMessage.addListener(
	function(request, sender, sendResponse) {
		chrome.browserAction.setBadgeText({text: request.removedElements});
	});