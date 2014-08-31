
// OmniHelp functions for toc construction, OHver 0.1
var lastTocShow = -1

function useToc() {
	currNavCtrl = 0
	writeTocCtrlFile()
	writeTocDocFile()
	setCookies()
}


function writeTocCtrlFile() {
	if ((tocShow == lastTocShow) && (lastNavCtrl == 0)) {
		return
	}
	lastNavCtrl = 0
	lastTocShow = tocShow
	var doc = parent.navctrl.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Nav Control: Contents, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="ctrl"><table border="0" cellpadding="2">\n')
	writeTocCtrl(doc)
	doc.write('\n</table></body></html>')
	doc.close()
}


function writeTocCtrl(doc) {
	var str = '<tr>'
	ctrlCount = 1
	str = '<th class="navx" width="50">Contents</th>'
	if (includeIx) {
		ctrlCount++
		str += '<th class="nav" width="40"><a href="javascript:parent.ctrl.useIx()">Index</a></th>'
	}
	if (includeSearch) {
		ctrlCount++
		str += '<th class="nav" width="40"><a href="javascript:parent.ctrl.useSearch()">Search</a></th>'
	}
	if (includeRel) {
		ctrlCount++
		str += '<th class="nav" width="50"><a href="javascript:parent.ctrl.useRel()">Related</a></th>'
	}
	str += '</tr><tr><td class="navx"'
	if (ctrlCount > 1) {
		str += 'colspan="' + ctrlCount + '"'
	}
	str += '><form name="navContents" id="navContents">'
	str += '<tr><td class="navxsmall" colspan="4">'
	str += '<input type="radio" name="tocext" value="0" '
	if (tocShow == 0) {
		str += 'checked="checked" '
	}
	str += 'onClick="javascript:parent.ctrl.setTocDisplay(0)" />Short&nbsp;&nbsp;'
	str += '<input type="radio" name="tocext" value="1" '
	if (tocShow == 1) {
		str += 'checked="checked" '
	}
	str += 'onClick="javascript:parent.ctrl.setTocDisplay(1)" />Medium&nbsp;&nbsp;'
	str += '<input type="radio" name="tocext" value="2" '
	if (tocShow == 2) {
		str += 'checked="checked" '
	}
	str += 'onClick="javascript:parent.ctrl.setTocDisplay(2)" />Full'
	str += '</td></tr></form>'
	doc.writeln(str)
}


function writeTocDocFile() {
	if ((tocShow == lastTocShow) && (lastNavDoc == 0) && (lastTocId == currTocId)) {
		return
	}
	//lastTocShow = tocShow
	lastNavDoc = 0
	lastTocId = currTocId
	var doc = parent.nav.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Contents File, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="nav"')
	if (currTocId == -1) {
		doc.write('>\n<p class="toc1">Document not in contents list.</p>')
	} else {
		doc.write(' onload="parent.ctrl.tocLoaded(document)">\n')
		writeTocItems(doc)
	}
	doc.write('\n</body></html>')
	doc.close()
}


function tocLoaded(doc) {
	// if curr anchor is not in view, scroll down so it is centered
	var vpos = 0
	var vhigh = 0

	if (parent.isN4) {
		vpos = doc.anchors[0].y
	} else if (parent.isIE4) {
		vpos = doc.all.curr.offsetTop
	} else {
		vpos = doc.getElementById("curr").offsetTop
	}

	if (parent.isNav) {
		vhigh = parent.nav.innerHeight
	} else {
		vhigh = doc.body.clientHeight
	}

	if (vpos > (vhigh - 50)) {
		parent.nav.scrollTo(0, vpos - (vhigh / 3))
	}
}


function setTocDisplay(val) {
	tocShow = val
	parent.navctrl.document.navContents.tocext[tocShow].checked = true
	writeTocDocFile()
	setCookies()
}

// functions for toc construction

function writeTocItems(doc) {
	if (currTocId == -1) {
		checkTocItem()
	}

	var itemNum = currTocId
	var itemDat = tocItems[itemNum]
	var itemSel = itemNum
	var itemLev = itemDat[0]
	var currLev = itemLev
	var prevSibs = true
	var ownChildren = true
	var itemList = new Array()
	var tocItemListPos = -1

	// scan back for sibs and ancestors
	for (itemNum = (itemSel - 1) ; itemNum >= 0 ; itemNum--) {
		if (tocShow > 1) {
			itemList.unshift(itemNum)
		} else {
			itemDat = tocItems[itemNum]
			if (tocShow && prevSibs && (itemDat[0] == itemLev)) {
				itemList.unshift(itemNum)
			}
			if (itemDat[0] < currLev) {
				prevSibs = false
				itemList.unshift(itemNum)
				currLev = itemDat[0]
				if (currLev == 1) {
					break
				}
			}
		}
	}

	// write selected item
	itemList.push(itemSel)
	tocItemListPos = itemList.length - 1

	// write following sibs and direct children
	for (itemNum = (itemSel + 1) ; itemNum < tocItems.length ; itemNum++) {
		if (tocShow < 2) {
			itemDat = tocItems[itemNum]
			currLev = itemDat[0]
			if (currLev < itemLev) {   // higher-level head
				itemList.push(itemNum)   // include it
				break
			}
			if (currLev == itemLev) {  // next head at same level
				if (!tocShow && (ownChildren == false)) {
					continue
				}
				ownChildren = false
			}
			if (!ownChildren && (currLev > itemLev)) {  // nephews
				continue
			}
		}
		itemList.push(itemNum)
	}
	
	// write out scanned items
	for (var i = 0; i < itemList.length; i++) {
		writeOneTocItem(doc, itemList[i], (i == tocItemListPos) ? true : false)
	}
}

function writeOneTocItem(doc, num, curr) {
	if ((num < 0) || !checkCondToc(num)) {
		return
	}
	var str = '<p class="toc' + tocItems[num][0]
	if (curr) {
		str += 'x"><a name="curr"></a>'
	} else {
		str += '">'
	}
	str += '<a href="javascript:parent.ctrl.setDoc(' + num + ')">'
	str += tocItems[num][1] + '</a></p>'
	doc.writeln(str)
}


// end of toccode.js

