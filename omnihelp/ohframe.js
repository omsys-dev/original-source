
// OmniHelp JS code referenced in head of frameset file, OHver 0.1


// filename for initial load, uses first in toc if empty
var mainName = ""
var hashName

// files loaded first
var topNavName = "topnav.htm"
var ctrlName = "ctrl.htm"

// CSS files to be used based on browser detection
var ctrlCssName = "ctrl.css"
var mainCssName = "ugns4.css"

var IECtrlCssName = "ctrl.css"
var N6CtrlCssName = "ctrl.css"
var NavCtrlCssName = "ctrl.css"

var IECssName = "ugie.css"
var N6CssName = "ugns6.css"
var NavCssName = "ugns4.css"


// functions used to set CSS per browser type

var setCSS = true
var isNav = true
var isN4 = true
var isN6 = false
var isIE = false
var isIE4 = false

function selectCSS() {
	var ver = parseInt(navigator.appVersion)
	if (ver >= 4) {
		isNav = (navigator.appName == "Netscape")
		isN6 = (parseInt(navigator.appVersion.substring(0,1)) >= 5)
		isIE = (navigator.appName.indexOf("Microsoft") != -1)
		isN4 = isNav && !isN6 && !isIE
		isIE4 = isIE && (ver == 4)
	}

	if (isIE) {
		mainCssName = IECssName
		ctrlCssName = IECtrlCssName
	} else if (isN6) {
		mainCssName = N6CssName
		ctrlCssName = N6CtrlCssName
	} else if (isNav) {
		mainCssName = NavCssName
		ctrlCssName = NavCtrlCssName
	}
	setCSS = false
}

function ctrlCSS(doc) {
	if (setCSS) {
		selectCSS()
	}
	var str = '<link rel="stylesheet" href="' + ctrlCssName + '" type="text/css" />'
	doc.writeln(str)
}

function mainCSS(doc) {
	if (setCSS) {
		selectCSS()
	}
	var str = '<link rel="stylesheet" href="' + mainCssName + '" type="text/css" />'
	doc.writeln(str)
}


// function used to produce empty files for initial load

function blank() {
	return "<html></html>"
}


function frameLoaded() {
	if (window.location.hash) {
		hashName = window.location.hash.substr(1)
	}
	topnav.location.href = topNavName
	ctrl.location.href = ctrlName
}

// end of ctrlset.js

