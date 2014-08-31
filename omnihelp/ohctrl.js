
// OmniHelp main JavaScript control functions, OHver 0.1

// usage for each possible navigation element
var includeToc = true
var includeIx = true
var includeSearch = true
var includeRel = true

// variables used for internal state info
var ctrlCount = 4
var currNavCtrl = 0
var lastNavCtrl = -1
var lastNavDoc = -1
var currTocId = 0
var lastTocId = 0
var currIxId = "0"
var lastIxId = ""
var lastDocIxId = ""
var tocShow = 1
var lastTocShow = -1
var boolSearch = "new"
var lastBoolSearch = "new"
var lastSearch = ""
var firstLoad = true
var lastMainFile = ""


// functions used to load frames in sequence

function loadData(doc)
{
	// always include toc data even if not displaying toc itself
	var str = '<script language="JavaScript" type="text/javascript" src="tocdata.js"><'
	str += '\/script>'
	doc.writeln(str)
	if (includeToc) {
		str = '<script language="JavaScript" type="text/javascript" src="toccode.js"><'
		str += '\/script>'
		doc.writeln(str)
	}
	if (includeIx) {
		str = '<script language="JavaScript" type="text/javascript" src="ixcode.js"><'
		str += '\/script>'
		doc.writeln(str)
		str = '<script language="JavaScript" type="text/javascript" src="ixdata.js"><'
		str += '\/script>'
		doc.writeln(str)
	}
	if (includeSearch) {
		str = '<script language="JavaScript" type="text/javascript" src="srchcode.js"><'
		str += '\/script>'
		doc.writeln(str)
		str = '<script language="JavaScript" type="text/javascript" src="srchdata.js"><'
		str += '\/script>'
		doc.writeln(str)
	}
	if (includeRel) {
		str = '<script language="JavaScript" type="text/javascript" src="relcode.js"><'
		str += '\/script>'
		doc.writeln(str)
		str = '<script language="JavaScript" type="text/javascript" src="reldata.js"><'
		str += '\/script>'
		doc.writeln(str)
	}
}

function ctrlLoaded() {
	getCookies()
	var origTocId = currTocId
	if (parent.hashName) {
		checkTocItem(parent.hashName)
		if (currTocId == -1) {
			currTocId = origTocId
		}
		else {
			setCookies()
		}
	}
	writeNavCtrlFile()
	setTimeout("writeMainFile()", 1000)
}

function writeMainFile() {
	if (!parent.mainName) {
		parent.mainName = tocItems[0][2]
	}
	if (parent.hashName) {
		lastMainFile = parent.hashName
		parent.hashName = ""
	} else if (currTocId <= 0) {
		lastMainFile = parent.mainName
	} else {
		lastMainFile = tocItems[currTocId][2]
	}
	parent.main.location.replace(lastMainFile)
	writeNavDocFile(true)
	setCookies()
}

function mainChanged() {
	getCookies()
	if (checkTocItem()) {
		setCookies()
		writeNavDocFile(firstLoad)
		firstLoad = false
	}
}

function writeNavCtrlFile() {
	if (currNavCtrl == 0) {
		if (includeToc) {
			writeTocCtrlFile()
		} else {
			currNavCtrl = 1
		}
	}
	if (currNavCtrl == 1) {
		if (includeIx) {
			writeIxCtrlFile()
		} else {
			currNavCtrl = 2
		}
	}
	if (currNavCtrl == 2) {
		if (includeSearch) {
			writeSearchCtrlFile()
		} else {
			currNavCtrl = 3
		}
	}
	if (currNavCtrl == 3) {
		if (includeRel) {
			writeRelCtrlFile()
		} else {
			currNavCtrl = 0
			writeTocCtrlFile()
		}
	}
}

function writeNavDocFile(all) {
	if (currNavCtrl == 0) {
		writeTocDocFile()
	} else if (all && (currNavCtrl == 1)) {
  	writeIxDocFile()
	} else if (all && (currNavCtrl == 2)) {
  	writeSearchDocFile(false)
	} else if (currNavCtrl == 3) {
  	writeRelDocFile()
	}
}


// functions to store and retrieve current state in cookies

function getCookies() {
	var str = unescape(document.cookie)
	var pos = str.indexOf("=")
	if (pos == -1) {
		return false
	}
	var start = 0
	var end = str.indexOf(";")
	while (end != -1) {
		getCookVal(str.substring(start, end))
		start = end + 2
		end = str.indexOf(";", start)
	}
	getCookVal(str.substring(start, str.length))
	return true
}

function getCookVal(cook) {
	var pos = cook.indexOf("=")
	if (pos == -1) {
		return
	}
	var name = cook.substring(0, pos)
	var val = cook.substring(pos + 1)
	if (name == "currTocId") {
		currTocId = parseInt(val)
	} else if (name == "currNavCtrl") {
		currNavCtrl = parseInt(val)
	} else if (name == "lastSearch") {
		lastSearch = val
	} else if (name == "boolSearch") {
		boolSearch = val
	} else if (name == "tocShow") {
		tocShow = parseInt(val)
	} else if (name == "currIxId") {
		currIxId = val
	} else if (name == "mainName") {
		parent.mainName = unescape(val)
	}
}

function setCookies() {
	var exp = new Date()
	var nextYear = exp.getTime() + (365 * 24 * 60 * 60 * 1000)
	exp.setTime(nextYear)
	var str = '; expires=' + exp.toGMTString()
	document.cookie = "currTocId=" + currTocId + str
	document.cookie = "currNavCtrl=" + currNavCtrl + str
	document.cookie = "lastSearch=" + lastSearch + str
	document.cookie = "boolSearch=" + boolSearch + str
	document.cookie = "tocShow=" + tocShow + str
	document.cookie = "currIxId=" + currIxId + str
	document.cookie = "mainName=" + escape(parent.mainName) + str
}



// functions for topnav control buttons

function getStart() {
	currTocId = 0
	currNavCtrl = 0
	firstLoad = true
	if (!parent.mainName) {
		parent.mainName = tocItems[0][2]
	} else {
		checkTocItem(parent.mainName)
		if (currTocId == -1) {
			currTocId = 0
		}
	}
	setDoc(currTocId, parent.mainName)
	writeTocCtrlFile()
	writeTocDocFile()
}

function getPrev() {
	checkTocItem()
	if (currTocId < 0) { // invalid, no idea what is prev
		getStart()
	}

	var sel = currTocId
	if (sel > 0) {
		sel--
	}
	var item = tocItems[sel]
	while (sel > 0) {
		if (checkCondToc(item)) {
			break
		}
		sel--
		item = tocItems[sel]
	}
	setDoc(sel)
}

function getNext() {
	checkTocItem()
	if (currTocId < 0) { // invalid, no idea what is next
		getStart()
	}

	var sel = currTocId
	var prev = tocItems[sel]
	if (sel < (tocItems.length - 1)) {
		sel++
	}
	var item = tocItems[sel]
	while (sel < (tocItems.length - 1)) {
		if ((item[2] != prev[2]) && checkCondToc(item)) {
			break
		}
		sel++
		item = tocItems[sel]
	}
	setDoc(sel)
}


function setDoc(num, ref) {
	if (num == currTocId) { 
		if (!firstLoad) {
			return
		}
		firstLoad = false
	} else if (num == -1) {
		currTocId = -1
		return
	}
	parent.main.location.href = ref ? ref : tocItems[num][2]
	lastMainFile = parent.main.location.href
	currTocId = num
	writeNavDocFile(false)
	setCookies()
}



function checkTocItem(url) {

	if (!url) {
		url = parent.main.location.href
	}
	var pathEnd = url.lastIndexOf("/") + 1
	var currFile = unescape(url.substring(pathEnd, url.length))

	if ((currTocId >= 0) && (tocItems[currTocId][2] == currFile)) {
	  // correct Id, no need to reload
		return false
	}

	// otherwise change currTocId to match the file loaded
	var itemNum = 0
	for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
		if (tocItems[itemNum][2] == currFile) {
			currTocId = itemNum
			// found in list, reload
			return true
		}		
	}

	// try again, matching base name only
	var hash = url.indexOf("#")
	if (hash == -1) {
		hash = url.length
	}
	currFile = unescape(url.substring(pathEnd, hash))
	currFile = currFile.toLowerCase()
	var itemName = ""
	var baseName = ""
	for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
		itemName = tocItems[itemNum][2]
		hash = itemName.indexOf("#")
		if (hash == -1) {
			hash = itemName.length
		}
		baseName = itemName.substring(0, hash)
		baseName = baseName.toLowerCase()
		if (baseName == currFile) {
			currTocId = itemNum
			// found in list, reload
			return true
		}		
	}

	// mark as invalid (may be external Web page)
	currTocId = -1
	// not in list, no use reloading
	return false
}


// functions for secondary and pop-up windows

var nextPop = 0
var popProps = "dependent,scrollbars,resizable"
var popWins = new Array()
var secProps = "dependent,scrollbars,resizable,title,titlebar,status,menubar,toolbar"
var secWins = new Array()


function secWin(link, wide, high, props) {
	// called from onClick handler in <a> link tag
	var win = link.target
	var url = link.href
	var nwin = 0
	var pprop = ""

	if (win == "popup") {  // make fresh each time
		nextPop++
		var pname = 'pop' + nextPop
		pprop = props ? props : popProps
		if (wide) {
			pprop += ",width=" + wide
		}
		if (high) {
			pprop += ",height=" + high
		}
		nwin = window.open(url, pname, pprop)
		if (nwin) {
			popWins[nextPop] = nwin
		}
	} else {  // secondary window, re-use if open
		var found = false
		var loaded = false
		var len = secWins.length
		var i = 0

		for (i = 0 ; i < len ; i++) {
			if (secWins[i][0] == win) {
				found = true
				// if still open, use it
				nwin = secWins[i][1]
				if (!nwin.closed) {
					nwin.location.href = url
					loaded = true
				}
				break
			}
		}
		if (!loaded) {
			pprop = props ? props : secProps
			if (wide) {
				pprop += ",width=" + wide
			}
			if (high) {
				pprop += ",height=" + high
			}
			nwin = window.open(url, win, pprop)
			if (nwin) {
				if (found) {
					secWins[i][1] = nwin
				} else {
					secWins[len] = new Array(win, nwin)
				}
			}
		}
	}
}


// stub functions for cond:

function checkCondToc(num) {
	return true
}

function checkCondIx(num) {
	return true
}

// end of ctrl.js
