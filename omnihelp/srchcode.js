
// OmniHelp functions for search, OHver 0.1

var foundSearch = new Array()
var foundLastSearch = false
var didSearch = false
var lastNewSearch = ""


function useSearch() {
	currNavCtrl = 2
	writeSearchCtrlFile()
	writeSearchDocFile(false)
	setCookies()
}


function writeSearchCtrlFile() {
	if ((boolSearch == lastBoolSearch) && (lastNavCtrl == 2)) {
		return
	}
	lastNavCtrl = 2
	lastBoolSearch = boolSearch
	var doc = parent.navctrl.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Nav Control: Search, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="ctrl"><table border="0" cellpadding="2">\n')
	writeSearchCtrl(doc)
	doc.write('\n</table></body></html>')
	doc.close()
}

function writeSearchCtrl(doc) {
	var str = '<tr>'
	ctrlCount = 1
	if (includeToc) {
		ctrlCount++
		str += '<th class="nav" width="50"><a href="javascript:parent.ctrl.useToc()">Contents</a></th>\n'
	}
	if (includeIx) {
		ctrlCount++
		str += '<th class="nav" width="40"><a href="javascript:parent.ctrl.useIx()">Index</a></th>\n'
	}
	str += '<th class="navx" width="40">Search</th>\n'
	if (includeRel) {
		ctrlCount++
		str += '<th class="nav" width="50"><a href="javascript:parent.ctrl.useRel()">Related</a></th>\n'
	}
	str += '</tr><form name="navSearch" method="post" action="javascript: void parent.ctrl.doSearch()">\n'
	str += '<tr><td class="navxsmall" colspan="4">&nbsp;\n'
	str += '<input type="button" value="List" onClick="javascript:parent.ctrl.doList()" />&nbsp;\n'
	str += '<input type="text" name="searchstring"'
	if (lastSearch) {
		str += ' value="' + lastSearch + '"'
	}
	str += ' size="8" />&nbsp;&nbsp;\n'
	str += '<input type="submit" value="Find" onClick="javascript:void parent.ctrl.doSearch(); return false" />\n'
	str += '</td></tr><tr><td class="navxsmall" colspan="4">\n'
	str += '<input type="radio" name="bools" value="new"'
	if (boolSearch == "new") {
		str += 'checked="checked" '
	}
	str += ' onclick="javascript:parent.ctrl.setSearch(\'new\')" />New&nbsp;\n'
	str += '<input type="radio" name="bools" value="and"'
	if (boolSearch == "and") {
		str += 'checked="checked" '
	}
	str += ' onclick="javascript:parent.ctrl.setSearch(\'and\')" />AND&nbsp;\n'
	str += '<input type="radio" name="bools" value="or"'
	if (boolSearch == "or") {
		str += 'checked="checked" '
	}
	str += ' onclick="javascript:parent.ctrl.setSearch(\'or\')" />OR&nbsp;\n'
	str += '<input type="radio" name="bools" value="not"'
	if (boolSearch == "not") {
		str += 'checked="checked" '
	}
	str += ' onclick="javascript:parent.ctrl.setSearch(\'not\')" />NOT\n'
	str += '</td></tr></form>\n'
	doc.write(str)
}

function setSearch(str) {
	boolSearch = str
	setCookies()
}


function doList() {
	writeSearchDocFile(true)
}


function doSearch() {
	lastSearch = parent.navctrl.document.navSearch.searchstring.value
	didSearch = true
	writeSearchDocFile(false)
	setCookies()
}


function writeSearchDocFile(list) {
	//if ((boolSearch == lastBoolSearch) && (lastNavDoc == 2)) {
	//	return
	//}
	lastNavDoc = 2
	//lastBoolSearch = boolSearch
	var doc = parent.nav.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Search File, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="nav">\n')
	if (list) {
		writeSearchTermList(doc)
	} else if (didSearch) {
		writeSearchItems(doc)
	} else {
		doc.write('<p class="toc1">Enter search term in box above.</p>')
	}
	doc.write('\n</body></html>')
	doc.close()
	setTimeout("parent.nav.scrollTo(0,0)", 1000)
}


function writeSearchItems(doc) {
	var itemNum = 0
	var searchStr = lastSearch.toLowerCase()
	var wordDat = new Array()
	var searchWord = new Array()
	var searchWordFound = false
	var searchWordPos = 0

	foundLastSearch = false

	// find array for word in searchWords[]
	for (itemNum = 0 ; itemNum < searchItems.length ; itemNum++) {
		wordDat = searchItems[itemNum]
		if (searchStr == wordDat[0]) {
			searchWord = wordDat[1]
			searchWordFound = true
			searchWordPos = 0
			break
		}
	}

	if (!lastNewSearch && (boolSearch != "new")) {
		boolSearch = "new"
		parent.navctrl.document.navSearch.bools[0].checked = true
	}

	if (boolSearch == "new") {
		lastNewSearch = searchStr
	} else {
		lastNewSearch += ' ' + boolSearch.toUpperCase() + ' ' + searchStr
	}

	if (searchWordFound) {
		if (boolSearch == "new") {
			for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
				foundSearch[itemNum] = false
				if ((searchWordPos < searchWord.length) && (itemNum == searchWord[searchWordPos])) {
					if (checkCondToc(itemNum)) {
						foundSearch[itemNum] = true
						writeOneSearchItem(doc, itemNum)
					}
					searchWordPos++
				}						
			}
		} else if (boolSearch == "and") {
			for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
				if (foundSearch[itemNum]) {
					while ((searchWordPos < searchWord.length) && (itemNum > searchWord[searchWordPos])) {
						searchWordPos++
					}
					if ((searchWordPos >= searchWord.length) || (itemNum < searchWord[searchWordPos])) {
						foundSearch[itemNum] = false
					} else {
						writeOneSearchItem(doc, itemNum)
						searchWordPos++
					}
				}
			}
		} else if (boolSearch == "or") {
			for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
				if (foundSearch[itemNum]) {
					writeOneSearchItem(doc, itemNum)
				} else {
					if ((itemNum == searchWord[searchWordPos]) && checkCondToc(itemNum)) {
						foundSearch[itemNum] = true
						writeOneSearchItem(doc, itemNum)
						searchWordPos++
					}
				}
			}
		} else if (boolSearch == "not") {
			for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
				if (foundSearch[itemNum]) {
					while ((searchWordPos < searchWord.length) && (itemNum > searchWord[searchWordPos])) {
						searchWordPos++
					}
					if ((searchWordPos < searchWord.length) && (itemNum == searchWord[searchWordPos])) {
						foundSearch[itemNum] = false
						searchWordPos++
					} else {
						writeOneSearchItem(doc, itemNum)
					}
				}
			}
		}
	}

	if (!foundLastSearch) {
		if (boolSearch != "new") {
			boolSearch = "new"
			parent.navctrl.document.navSearch.bools[0].checked = true
		}
		doc.write('<p class="toc1">No search results for "')
		doc.write(lastNewSearch)
		doc.writeln('".</p>')
	}

}


function writeOneSearchItem(doc, num) {
	if (!foundLastSearch) {
		doc.write('<p class="toc1">Search results for "')
		doc.write(lastNewSearch)
		doc.writeln('":<br /></p>')
		foundLastSearch = true
	}

	var str = '<p class="toc1"><a href="javascript:parent.ctrl.setDoc(' + num + ')">'
	str += tocItems[num][1] + '</a></p>'
	doc.write(str)
}


function writeSearchTermList(doc) {
	var itemNum = 0
	var wordDat = new Array()
	var str

	doc.write('<p class="toc1">Search terms:<br>')
	for (itemNum = 0 ; itemNum < searchItems.length ; itemNum++) {
		wordDat = searchItems[itemNum]
		str = '<br><a href="javascript: void parent.ctrl.setTerm('
		if (parent.isN4) {
			str += '\''+ wordDat[0] + '\''
		} else {
			str += 'this'
		}
		str += ')">' + wordDat[0] + ' (' + wordDat[1].length + ')</a>\n'
		doc.write(str)
	}
	doc.write('</p>')
}

function setTerm(link) {
	if (parent.isN4) {
		lastSearch = link
	} else {
		lastSearch = link.innerHTML
	}
	parent.navctrl.document.navSearch.searchstring.value = lastSearch
	setCookies()
}


// end of srchcode.js

