
// OmniHelp ctrl functions for related (ALinks), OHver 0.1

function useRel() {
	currNavCtrl = 3
	writeRelCtrlFile()
	writeRelDocFile()
	setCookies()
}


function writeRelCtrlFile() {
	if (lastNavCtrl == 3) {
		return
	}
	lastNavCtrl = 3
	var doc = parent.navctrl.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Nav Control: Related, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="ctrl"><table border="0" cellpadding="2">\n')
	writeALinkCtrl(doc)
	doc.write('\n</table></body></html>')
	doc.close()
}

function writeALinkCtrl(doc) {
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
	if (includeSearch) {
		ctrlCount++
		str += '<th class="nav" width="40"><a href="javascript:parent.ctrl.useSearch()">Search</a></th>\n'
	}
	str += '<th class="navx" width="50">Related</th></tr>\n'
	str += '<tr><td class="navx"'
	if (ctrlCount > 1) {
		str += 'colspan="' + ctrlCount + '"'
	}
	str += '><p class="navx">Topics related by subject to the displayed topic.</p></td></tr>\n'
	doc.write(str)
}


function writeRelDocFile() {
	if ((lastNavDoc == 3) && (lastTocId == currTocId)) {
		return
	}
	lastNavDoc = 3
	lastTocId = currTocId
	var doc = parent.nav.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Related Topics File, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="nav">\n')
	if (currTocId == -1) {
		doc.write('<p class="toc1">Document not in related lists.</p>')
	} else {
		writeALinks(doc)
	}
	doc.write('\n</body></html>')
	doc.close()
	setTimeout("parent.nav.scrollTo(0,0)", 1000)
}

function numcomp(a, b) {
	return a - b
}

function writeALinks(doc) {
	var itemNum = 0
	var relItemDat = new Array()
	var itemSel = currTocId
	var topicItems = new Array()
	var idx = 0
	var idx2 = 0
	var relTopics = new Array()
	var relPos = 0
	var found = false

	// go through all alink sets for those that ref the current topic
	// add all members of those sets to relItems (except current topic)
	for (itemNum = 0 ; itemNum < relItems.length ; itemNum++ ) {
		relItemDat = relItems[itemNum]
		topicItems = relItemDat[1]
		for (idx = 0 ; idx < topicItems.length ; idx++) {
			if (topicItems[idx] == itemSel) {
				for (idx2 = 0 ; idx2 < topicItems.length ; idx2++) {
					if (topicItems[idx2] != itemSel) {
						relTopics.push(topicItems[idx2])
					}
				}
				break
			}
		}
	}

	if (relTopics.length > 0) {
		relTopics.sort(numcomp)
		// go through TOC and display any topics found in relItems
		for (itemNum = 0 ; itemNum < tocItems.length ; itemNum++) {
			while (itemNum > relTopics[relPos]) {  // must be duplicate
				if (++relPos >= relTopics.length) {
					break
				}
			}
			if (relPos >= relTopics.length) {
				break
			}
			if (itemNum == relTopics[relPos]) {
				writeOneRelItem(doc, itemNum)
				found = true
				if (++relPos >= relTopics.length) {
					break
				}
			}
		}
	}

	if (!found) {
		doc.write('<p class="toc1">No related topics found.</p>')
	}
}


function writeOneRelItem(doc, num) {
	var str = '<p class="toc1"><a href="javascript:parent.ctrl.setDoc(' + num + ')">'
	str += tocItems[num][1] + '</a></p>'
	doc.writeln(str)
}


// end of relcode.js
