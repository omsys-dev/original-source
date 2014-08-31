
var itemNum = 0
var itemLev = 1
var itemSel = 0
var itemDat = new Array()
var itemList = new Array()
var prevSibs = true
var ownChildren = true
var currLev = 1
var currFile

function writeTocItems() {

	if (parent.ctrl.clickInToc) {
		itemNum = parent.ctrl.currTocId
		parent.ctrl.clickInToc = false
		itemDat = parent.work.tocItems[itemNum]
	} else {
		var url = parent.main.location.href
		currFile = unescape(url.substring((url.lastIndexOf("/") + 1), url.length))
		for (itemNum = 0 ; itemNum < parent.work.tocItems.length ; itemNum++) {
			itemDat = parent.work.tocItems[itemNum]
			if (itemDat[2] == currFile) {
				break
			}		
		}
		if (itemNum == parent.work.tocItems.length) {
			return false
		}
	}
	parent.work.tocSel = itemNum
	itemSel = itemNum
	itemLev = itemDat[0]
	currLev = itemLev
	prevSibs = true
	ownChildren = true

	for (itemNum = (itemSel - 1) ; itemNum >= 0 ; itemNum--) {
		// scan back for sibs and ancestors
		itemDat = parent.work.tocItems[itemNum]
		if ((prevSibs == true) && (itemDat[0] == itemLev)) {
			itemList.push(itemNum)
		}
		if (itemDat[0] < currLev) {
			prevSibs = false
			itemList.push(itemNum)
			currLev = itemDat[0]
			if (currLev == 1) {
				break
			}
		}
	}
	while (itemList.length > 0) {
		// write out scanned items
		itemNum = itemList.pop()
		itemDat = parent.work.tocItems[itemNum]
		writeOneTocItem(false)
	}
	// write selected item
	itemNum = itemSel
	itemDat = parent.work.tocItems[itemNum]
	writeOneTocItem(true)
	for (itemNum = (itemSel + 1) ; itemNum < parent.work.tocItems.length ; itemNum++) {
		// write following sibs and direct children
		itemDat = parent.work.tocItems[itemNum]
		if (itemDat[0] < itemLev) {
			return true
		}
		if (itemDat[0] == itemLev) {
			ownChildren = false
		}
		if ((ownChildren == false) && (itemDat[0] > itemLev)) {
			continue
		}
		writeOneTocItem(false)
	}
}


function writeOneTocItem(curr) {
	document.write('<p class="toc')
	document.write(itemDat[0])
	if (curr == true) {
		document.write('x">')
		document.write(itemDat[1])
	} else {
		document.write('"><a href="')
		document.write(itemDat[2])
		document.write('" target="main" onClick="setTocId(')
		document.write(itemNum)
		document.write(')">')
		document.write(itemDat[1])
		document.write('</a>')
	}
	document.writeln('</p>')
}

function setTocId(num) {
	parent.ctrl.currTocId = num
	parent.ctrl.clickInToc = true
	parent.nav.location.replace(parent.ctrl.currToc)
	parent.ctrl.clickInToc = true
	parent.ctrl.noTocReload = true
}


if (parent.work.tocItems) {
	if (parent.work.tocItems.length == 0) {
		document.writeln('<p class="toc1">Contents empty.</p>')
	} else {
		writeTocItems()
	}
} else {
	document.writeln('<p class="toc1">Contents not available.</p>')
}
