
var itemNum = 0
var itemDat = new Array()
var searchStr
var testStr

function writeQuickSearchItems() {
	var found = false
	searchStr = parent.ctrl.lastSearch.toLowerCase()
	for (itemNum = 0 ; itemNum < parent.work.tocItems.length ; itemNum++) {
		itemDat = parent.work.tocItems[itemNum]
		testStr = itemDat[1].toLowerCase()
		if (testStr.indexOf(searchStr) != -1) {
			writeOneSearchItem()
			found = true
		}		
	}
	if (found == false) {
		document.write('<p class="toc1">No search results for "')
		document.write(parent.ctrl.lastSearch)
		document.writeln('".</p>')
	}
}

function writeOneSearchItem() {
	document.write('<p class="toc1"><a href="')
	document.write(itemDat[2])
	document.write('" target="main">')
	document.write(itemDat[1])
	document.writeln('</a></p>')
}


if (parent.ctrl.quickSearch == true) {
	writeQuickSearchItems()
} else {
	document.writeln('<p class="toc1">Search results here</p>')
}